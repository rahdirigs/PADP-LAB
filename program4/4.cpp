#include <cstdio>
#include <gd.h>
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: ./4 <input_image> <output_image> <num_threads>\n");
    exit(0);
  }
  FILE *fp;
  FILE *fp1 = nullptr;
  char *iname = argv[1];
  char *oname = argv[2];
  int num_threads = atoi(argv[3]);
  if ((fp = fopen(iname, "r")) == nullptr) {
    fprintf(stderr, "Error: fopen() - %s\n", iname);
    exit(0);
  }
  gdImagePtr img = gdImageCreateFromPng(fp);
  int width = gdImageSX(img);
  int height = gdImageSY(img);
  int color, red, green, blue, avg, w, h;
  double start_time = omp_get_wtime();
#pragma omp parallel for private(h, color, red, green, blue, avg) schedule(static, num_threads)
  for (w = 0; w < width; ++w) {
    for (h = 0; h < height; ++h) {
      color = gdImageGetPixel(img, w, h);
      red = gdImageRed(img, color);
      green = gdImageGreen(img, color);
      blue = gdImageBlue(img, color);
      avg = (red + green + blue) / 3;
      color = gdImageColorAllocate(img, avg, avg, avg);
      gdImageSetPixel(img, w, h, color);
    }
  }
  double end_time = omp_get_wtime();
  cout << "Time taken = " << end_time - start_time << "s.\n";
  if ((fp1 = fopen(oname, "w")) == nullptr) {
    fprintf(stderr, "Error: fopen() - %s\n", oname);
    exit(0);
  }
  gdImagePng(img, fp1);
  gdImageDestroy(img);
  fclose(fp1);
  fclose(fp);
  return 0;
}

