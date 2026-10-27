#include <iostream>
#include <random>
#include <ctime>
#include <cuda_runtime.h>

__global__ void add(const float *a, const float *b, float *c, int N) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < N) {
    c[i] = a[i] + b[i];
  }
}

int main() {
  int N = 10000;
  size_t size = N * sizeof(float);

  // Allocate host memory
  float *h_a = (float *)malloc(size);
  float *h_b = (float *)malloc(size);
  float *h_c = (float *)malloc(size);

  // Populate input data
  std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
  for (int i = 0; i < N; i++) {
    h_a[i] = static_cast<float>(mt() % 100);
    h_b[i] = static_cast<float>(mt() % 100);
  }

  // Allocate device memory
  float *d_a, *d_b, *d_c;
  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, size);

  // Copy data from host to device
  cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

  // Configure launch parameters
  dim3 threads(256);
  dim3 blocks((N + threads.x - 1) / threads.x);

  // Launch kernel
  add<<<blocks, threads>>>(d_a, d_b, d_c, N);
  cudaDeviceSynchronize();

  // Copy output back to host
  cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

  // Print first 10 elements to verify
  for (int i = 0; i < 10; i++) {
    std::cout << h_a[i] << " + " << h_b[i] << " = " << h_c[i] << std::endl;
  }

  // Cleanup
  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_c);
  free(h_a);
  free(h_b);
  free(h_c);

  return 0;
}
