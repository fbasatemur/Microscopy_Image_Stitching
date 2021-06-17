# Microscopy Image Stitching
This repo is a solution for combining 2D images based on Phase Only correlation and Laplacian Pyramid methods. 
This method makes accurate and fast compositing only for 2D images moving in the x and y axes.
Multiple merging can be done using datasets consisting of images. In addition, the functions parallelized using OpenMP and still under development.

## Algorithmic process  
- Phase Only Correlation 
- Feature Matching
- Homography matrix
- Laplacian Pyramid 

### Acceleration using OpenMP
<img src="https://developers.redhat.com/blog/wp-content/uploads/2016/03/openmp_lg_transparent.gif" width="290" height="110" />


### It can generate panorama image even in small common areas.

![image 1](https://github.com/fbasatemur/Microscopy_Image_Stitching/blob/main/sample_images/little_area.jpg)

### Scanning can be done in all directions. It can combine images that have moved asymmetrically in all directions.

![image 2](https://github.com/fbasatemur/Microscopy_Image_Stitching/blob/main/sample_images/big_steps.jpg)

### Panoramic images created from sample microscope images

![image 3](https://github.com/fbasatemur/Microscopy_Image_Stitching/blob/main/sample_images/big_image1.bmp)

![image 4](https://github.com/fbasatemur/Microscopy_Image_Stitching/blob/main/sample_images/big_image2.bmp)

![image 5](https://github.com/fbasatemur/Microscopy_Image_Stitching/blob/main/sample_images/big_image3.bmp)