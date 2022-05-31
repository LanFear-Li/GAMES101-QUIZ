# GAMES101 Programming Assignment

## PA1 - Rotation and Projection

光栅化的第一步是进行观测变换(Viewing Transformation)。其中包括模型变换(Model Transformation)、视图/相机变换(View/Camera Transformation)以及投影变换(Projection Transformation)。在PA1中实现了：

**三维中绕$z$轴旋转的模型变换矩阵**`get_model_matrix (main.cpp)`

**透射投影矩阵**`get_projection_matrix (main.cpp)`

透射投影的构建分为两步，首先需要构造透射正交过渡矩阵$persp\_to\_ortho$，然后构造正交投影矩阵$ortho$，最终的投影矩阵$persp = ortho * persp\_to\_ortho$。

**效果**

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa1-1.png" style="zoom: 60%;text-align: left"/></p>

如图所示为经过光栅化后投影到屏幕的三角形，可通过AD键修改模型绕$z$轴旋转角度使三角形旋转。

## PA2 - Triangles and Z-buffering

光栅化一个实心三角形，需要渲染三角形内部的每一个像素，此时需要解决遍历整个三角形像素点的问题。在PA2中实现了：

**使用Z-buffer深度缓冲实现三角形光栅化**`rasterize_triangle (rasterizer.cpp)`

该算法维护两个buffer(Depth Buffer, Color Buffer)，分别存放各像素点的深度与颜色。

实现Z-buffer光栅化三角形，首先需要创建该三角形的二维bounding box，遍历边界框中的所有像素并判断是否在三角形内部，若在内部则计算出该点的插值深度值并和Depth Buffer在该点的值比较，若小于则更新Depth Buffer和Color Buffer在该点的值。

**判断点是否在三角形内部**`insideTriangle (rasterizer.cpp)` 

对于朴素做法：可通过判断点和三角形各顶点连线形成的向量与对应的边向量做叉乘得到，若三次叉乘在Z轴的方向相等，则表明点在三角形内部。

**效果**

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa2-1.png" style="zoom: 60%;text-align: left"/></p>

如图所示绿色三角形的深度小于蓝色三角形，故其覆盖了部分蓝色。

## PA3 - Pipeline and Shading

光栅化还需为模型着色，此时需要结合光源考虑模型各点的颜色、明暗以及可选的纹理。在PA3中实现了：

**使用Blinn-Phong光线反射模型实现顶点着色**`phong_fragment_shader (main.cpp)`

Blinn-Phong Reflection Model将点光源的作用分成三个部分：镜面反射、漫反射以及环境光照。对于前二者需要结合顶点到点光源的距离以及二者的夹角计算，环境光照则设置为常量。

**实现基本的纹理映射**`texture_fragment_shader (main.cpp)`

基本的Texture Mapping需要提供平面纹理图片以及三维模型的每个像素点与纹理图片像素点位置的对应关系。

**实现凹凸贴图**`bump_fragment_shader (main.cpp)`

**实现位移贴图**`bump_fragment_shader (main.cpp)`

凹凸贴图Bump Mapping以及位移贴图Displacement Mapping，这两种纹理映射方式在原先的基础上修改每个模型每个点在模型表面的位置，使得纹理更加真实。

**效果**

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa3-1.png" style="zoom: 60%;text-align: left"/></p>

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa3-2.png" style="zoom: 60%;text-align: left"/></p>

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa3-3.png" style="zoom: 60%;text-align: left"/></p>

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa3-4.png" style="zoom: 60%;text-align: left"/></p>

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa3-5.png" style="zoom: 60%;text-align: left"/></p>

## PA4 - Bezier Curve

我们通常需要各种形状的曲线来模拟出三维模型的外观，那么我们需要绘制可控制曲线的方法。在PA4中：

**实现了贝塞尔曲线的绘制**`recursive_bezier (main.cpp)`

首先需要了解一个概念：时间t。对于一条贝塞尔曲线，假设时间从t=0开始遍历曲线，到t=1遍历结束，那么对于为时间t(0 < t < 1)即对应了曲线中的某个点。

对于给定的n个控制点，递归地计算两两控制点的所谓时间t位置的中间点。当最终两个点的时间t位置的中间点计算完毕后，得到的点即为时间t下贝塞尔曲线对应的点。

**效果**

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa4-1.png" style="zoom: 60%;text-align: left"/></p>

## PA5 - Ray Triangle Intersection

光线追踪是和光栅化不同的做法，其渲染效果要优于光栅化，但是要花费更多的时间。其首要问题在于计算光线与物体是否存在交点。在PA5中：

**使用Moller-Trumbore算法判断光线和三角形是否存在交点**`rayTriangleIntersect (Triangle.hpp)`

对于光线和三角形的交点判断，朴素做法是首先计算光线在三角形所在平面点的坐标，然后判断该点是否在三角形内部即可。Moller-Trumbore利用三角形的重心坐标，将光线方程与三角形内部点的重心坐标方程联立，并根据光线和重心坐标的附加条件即可判断。

**效果**

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa5-1.png" style="zoom: 60%;text-align: left"/></p>

如图所示为光线追踪下光线多次折射/反射后的渲染效果。

## PA6 - Acceleration Structure

一个模型往往存在非常多个三角形曲面，若每根光线需要和所有三角形计算交点则会非常耗时，我们需要优化这个过程。我们可以对于空间划分，使用八叉树、KD树等空间数据结构优化所有曲面的存储与查找，也可以对于物体划分，使用BVH(Bounding Volume Hierarchy)完成优化。在PA6中：

**判断包围盒与光线是否相交**`IntersectP (Bound3.hpp)`

三维的包围盒(Axis-Aligned Bounding Boxes, AABB)类似于二维的Bounding Box，用于表示某个物体所在空间位置，包围盒各面与坐标轴平行，便于计算。

**使用BVH求出与光线的交点**`getIntersection (BVH.cpp)`

BVH是一种树形结构，该函数通过递归遍历BVH各节点的左右子节点并在叶节点求出物体与光线的交点实现。

**效果**

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa6-1.png" style="zoom: 50%;text-align: left"/></p>

## PA7 - Path Tracing

对于光线追踪，传统方式是Whitted-Style Ray Tracing，其在光线弹射的处理做了相应简化，规定只有高光材质的物体才会弹射光线。而在现实中这样显然是不准确的。通过辐射度量学我们可以精准定义空间某点汇聚光的能量以及正确处理光线弹射。在PA7中：

**实现路径追踪算法**`castRay (Scene.cpp)`

光线追踪的思路是从视点向屏幕的每个像素投射若干光线，计算每个光线第一个交点汇聚的光线能量。考虑该点的实际光线来源，可以分为两种情况：光线来自某一光源与光线来自某点的弹射，对于二者分别计算。

**效果**

<p align="center"><img src="https://raw.githubusercontent.com/LanFear-Li/GAMES101-QUIZ/main/Appendix/pa7-1.png" style="zoom: 50%;text-align: left"/></p>
