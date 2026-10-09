#include <iostream>

__global__ void add(float *a, float *b, float *c, int N) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < N) {
    c[i] = a[i] + b[i];
  }
}

int main() {
  int N = 10000;
  float *h_a, *h_b, *h_c;
  float *d_a, *d_b, *d_c;

  size_t size = N * sizeof(float);
  h_a = (float *)malloc(size);
  h_b = (float *)malloc(size);

  std::mt19937 mt(time(nullptr));
  for (int i = 0; i < N; i++) {
    h_a[i] = std::mt() % 100;
    h_b[i] = std::mt() % 100;
  }

  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, size);

  cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

  dim3 threads(32);
  dim3 N / threads;

  add<<<blocks, threads>>>(d_a, d_b, d_c, N);

  cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);

  for (int i = 0; i < N; i++) {
    std::cout << h_a[i] << ", " << h_b[i] << ", " << h_c[i] << std::endl;
  }
  std::cout << std::endl;

  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_c);
  free(h_a);
  free(h_b);
  free(h_c);
}
