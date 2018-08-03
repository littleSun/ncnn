// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2018 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "dequantize.h"

namespace ncnn {

    DEFINE_LAYER_CREATOR(Dequantize)

    Dequantize::Dequantize()
    {
        one_blob_only = true;
        support_inplace = true;
    }

    int Dequantize::load_param(const ParamDict& pd)
    {
        scale = pd.get(0, 1.f);
        bias_term = pd.get(1, 0);
        bias_data_size = pd.get(2, 0);

        return 0;
    }

    int Dequantize::load_model(const ModelBin& mb)
    {
        if (bias_term)
        {
            bias_data = mb.load(bias_data_size, 1);
            if (bias_data.empty())
                return -100;
        }

        return 0;
    }

    int Dequantize::forward_inplace(Mat& bottom_top_blob, const Option& opt) const
    {
        int dims = bottom_top_blob.dims;

        if (dims == 1)
        {
            fprintf(stderr, "dim=1 bias_data_size=%d\n", bias_data_size);
            int w = bottom_top_blob.w;

            int* intptr = bottom_top_blob;
            float* ptr = bottom_top_blob;

            if (bias_term)
            {
#pragma omp parallel for num_threads(opt.num_threads)
#if __APPLE__
                dispatch_apply(w, get_gcd_concurrent(), ^(size_t i) {
#else
                    for (int i=0; i<w; i++) {
#endif
                    ptr[i] = intptr[i] * scale + bias_data[i];
#if __APPLE__
                });
#else
                }
#endif
            }
            else
            {
#pragma omp parallel for num_threads(opt.num_threads)
#if __APPLE__
                dispatch_apply(w, get_gcd_concurrent(), ^(size_t i) {
#else
                    for (int i=0; i<w; i++) {
#endif
                    ptr[i] = intptr[i] * scale;
#if __APPLE__
                });
#else
                }
#endif
            }
        }

        if (dims == 3)
        {
            int w = bottom_top_blob.w;
            int h = bottom_top_blob.h;
            int channels = bottom_top_blob.c;
            int size = w * h;

            if (bias_term)
            {
#pragma omp parallel for num_threads(opt.num_threads)
#if __APPLE__
                dispatch_apply(channels, get_gcd_concurrent(), ^(size_t q) {
#else
                    for (int q=0; q<channels; q++) {
#endif
                    int* intptr = bottom_top_blob.channel(q);
                    float* ptr = bottom_top_blob.channel(q);

                    float bias = bias_data[q];

                    for (int i=0; i<size; i++)
                    {
                        ptr[i] = intptr[i] * scale + bias;
                    }
#if __APPLE__
                });
#else
                }
#endif
            }
            else
            {
#pragma omp parallel for num_threads(opt.num_threads)
#if __APPLE__
                dispatch_apply(channels, get_gcd_concurrent(), ^(size_t q) {
#else
                    for (int q=0; q<channels; q++) {
#endif

                    int* intptr = bottom_top_blob.channel(q);
                    float* ptr = bottom_top_blob.channel(q);

                    for (int i=0; i<size; i++)
                    {
                        ptr[i] = intptr[i] * scale;
                    }

#if __APPLE__
                });
#else
                }
#endif
            }
        }

        return 0;
    }

} // namespace ncnn