#include "util.h"
#include "maxflow-v3.0/graph.h"
#include <iostream>
#include <cmath>

// for 2d images
template<typename T>
T get_pixel(const T * data, int height, int width, int h, int w)
{
    return data[h * width + w];
}

template<typename T>
std::vector<T> get_pixel_vector(const T * data, int height, int width, int channel, int h, int w)
{
    std::vector<T> pixel_vector(channel);
    for (int c = 0; c < channel; c++){
        pixel_vector[c]= data[h * width * channel + w * channel + c];
    }
    return pixel_vector;
}

template<typename T>
void set_pixel(T * data,  int height, int width, int h, int w, T value)
{
    data[h * width + w] = value;
}

template<typename T>
float get_l2_distance(std::vector<T> p1, std::vector<T> p2)
{
    T sq_sum = 0.0;
    for(int d = 0; d < p1.size(); d++)
    {
        sq_sum = sq_sum + (p1[d] - p2[d]) * (p1[d] - p2[d]);
    }
    float dis = sqrt(sq_sum);
    return dis;
}

template
float get_pixel<float>(const float * data, int height, int width, int h, int w);

template
int get_pixel<int>(const int * data, int height, int width, int h, int w);

template
std::vector<float> get_pixel_vector<float>(const float * data, int height, int width, int channel, int h, int w);

template
unsigned char get_pixel<unsigned char>(const unsigned char * data, int height, int width, int h, int w);


template
void set_pixel<float>(float * data, int height, int width, int h, int w, float value);

template
void set_pixel<int>(int * data, int height, int width, int h, int w, int value);

template
void set_pixel<unsigned char>(unsigned char * data, int height, int width, int h, int w, unsigned char value);

template
float get_l2_distance(std::vector<float> p1, std::vector<float> p2);


// for 3d images
template<typename T>
T get_pixel(const T * data, int depth, int height, int width, int d, int h, int w)
{
    return data[(d*height + h) * width + w];
}

template<typename T>
std::vector<T> get_pixel_vector(const T * data, int depth,  int height, int width, int channel, int d, int h, int w)
{
    std::vector<T> pixel_vector(channel);
    for (int c = 0; c < channel; c++){
        pixel_vector[c]= data[d*height*width*channel +  h * width * channel + w * channel + c];
    }
    return pixel_vector;
}

template<typename T>
void set_pixel(T * data, int depth, int height, int width, int d, int h, int w, T value)
{
    data[(d*height + h) * width + w] = value;
}

template
float get_pixel<float>(const float * data, int depth, int height, int width, int d, int h, int w);

template
std::vector<float> get_pixel_vector<float>(const float * data, int depth, int height, int width, int channel, int d, int h, int w);

template
int get_pixel<int>(const int * data, int depth, int height, int width, int d, int h, int w);

template
unsigned char get_pixel<unsigned char>(const unsigned char * data,
                                       int depth, int height, int width,
                                       int d, int h, int w);


template
void set_pixel<float>(float * data, int depth, int height, int width, int d, int h, int w, float value);

template
void set_pixel<int>(int * data, int depth, int height, int width, int d, int h, int w, int value);

template
void set_pixel<unsigned char>(unsigned char * data,
                              int depth, int height, int width,
                              int d, int h, int w, unsigned char value);


void maxflow_inference(unsigned char * label, const float* img, const float * prob, const unsigned char * seed,
    int H, int W, int chns, int cls, float lambda, float sigma)
{
    // currently, only cls == 2 is supported
    typedef Graph<float, float, float> GraphType;
    /*estimated # of nodes*/ /*estimated # of edges*/
    GraphType * g = new GraphType(H*W, 2*H*W);
    g->add_node(H*W);
    float max_weight = -100000;
    for(int x=0; x<H;x++)
    {
        for(int y=0; y<W;y++)
        {
            std::vector<float> pValue = get_pixel_vector(img, H, W, chns, x, y);
            std::vector<float> qValue;
            float l2dis, n_weight;
            int pIndex  = x*W + y;
            int qIndex;
            int xn, yn;
            int Xoff[2] = {-1, 0};
            int Yoff[2] = {0, -1};
            for(int i=0; i<2; i++){
                xn = x + Xoff[i];
                yn = y + Yoff[i];
                if(xn < 0 || yn < 0) continue;
                qValue   = get_pixel_vector(img, H, W, chns, xn, yn);
                l2dis    = get_l2_distance(pValue, qValue);
                n_weight = lambda*exp(-l2dis * l2dis/(2*sigma*sigma));
                qIndex   = xn*W + yn;
                g->add_edge(qIndex,pIndex,n_weight,n_weight);
                if(n_weight > max_weight) max_weight = n_weight;
            }
        }
    }

    max_weight =  1000 * max_weight;
    for(int x=0;x<H;x++)
    {
        for(int y=0;y<W;y++)
        {
            bool seed_exist = false;
            float s_weight  = 1e-3;
            float t_weight  = 1e-3;
            if(seed != NULL)
            {
                std::vector<unsigned char> label = get_pixel_vector(seed, H, W, 2, x, y); 
                if(label[0] > 0){
                    t_weight   = max_weight;
                    seed_exist = true; 
                }
                else if(label[1] > 0){
                    s_weight   = max_weight;
                    seed_exist = true;
                }
            }
            if(!seed_exist){
                std::vector<float> probs = get_pixel_vector(prob, H, W, 2, x, y);
                s_weight = -log(probs[0]);
                t_weight = -log(probs[1]);
            }
           
            int  pIndex  = x*W + y;
            g->add_tweights(pIndex,s_weight,t_weight);
        }
    }
    double flow = g->maxflow();
    // cout<<"max flow: "<<flow<<endl; 
    for(int i=0; i<H*W; i++) label[i] = 1 - g->what_segment(i);
    delete g;
}

void maxflow3d_inference(unsigned char * label, const float* img, const float * prob, const unsigned char * seed,
    int D, int H, int W, int chns, int cls, float lambda, float sigma)
{
    // currently, only cls == 2 is supported
    typedef Graph<float, float, float> GraphType;
    /*estimated # of nodes*/ /*estimated # of edges*/
    GraphType * g = new GraphType(D*H*W, 2*D*H*W);
    g->add_node(D*H*W);
    float max_weight = -100000;
    for(int x=0; x<D; x++)
    {
        for(int y=0; y<H; y++)
        {
            for(int z=0; z<W; z++)
            {
                std::vector<float> pValue = get_pixel_vector(img, D, H, W, chns, x, y, z);
                std::vector<float> qValue;
                float l2dis, n_weight;
                int pIndex  = x*H*W + y*W + z;
                int qIndex;
                int xn, yn, zn;
                int Xoff[3] = {-1, 0, 0};
                int Yoff[3] = {0, -1, 0};
                int Zoff[3] = {0, 0, -1};
                for(int i=0; i<3; i++){
                    xn = x + Xoff[i];
                    yn = y + Yoff[i];
                    zn = z + Zoff[i];
                    if(xn < 0 || yn < 0 || zn < 0) continue;
                    qValue   = get_pixel_vector(img, D, H, W, chns, xn, yn, zn);
                    l2dis    = get_l2_distance(pValue, qValue);
                    n_weight = lambda*exp(-l2dis * l2dis/(2*sigma*sigma));
                    qIndex   = xn*H*W + yn*W + zn;
                    g->add_edge(qIndex,pIndex,n_weight,n_weight);
                    if(n_weight > max_weight) max_weight = n_weight;
                }
            }
        }
    }

    max_weight =  1000 * max_weight;
    for(int x=0; x<D; x++)
    {
        for(int y=0; y<H; y++)
        {
            for(int z=0; z<W; z++)
            {
                bool seed_exist = false;
                float s_weight  = 1e-3;
                float t_weight  = 1e-3;
                if(seed != NULL)
                {
                    std::vector<unsigned char> label = get_pixel_vector(seed, D, H, W, 2, x, y, z); 
                    if(label[0] > 0){
                        t_weight   = max_weight;
                        seed_exist = true; 
                    }
                    else if(label[1] > 0){
                        s_weight   = max_weight;
                        seed_exist = true;
                    }
                }
                if(!seed_exist){
                    std::vector<float> probs = get_pixel_vector(prob, D, H, W, 2, x, y, z);
                    s_weight = -log(probs[0]);
                    t_weight = -log(probs[1]);
                }
                int  pIndex  = x*H*W + y*W + z;
                g->add_tweights(pIndex,s_weight,t_weight);
            }
        }
    }
    double flow = g->maxflow();
    // cout<<"max flow: "<<flow<<endl;
    for(int i=0; i<D*H*W; i++) label[i] = 1 - g->what_segment(i);
    delete g;
}
