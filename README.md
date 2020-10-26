# Rapid-Digitization-Of-Articles

### Language, libraries and SDKS:

<pre>
1. C/C++
2. Win32 SDK (Windowing and event handling) //GlUT/GLFW can be used instead
3. Glew (OpenGL wrangler Library)
4. Glm (OpenGl assisting Lib)
5. OpenCV-3.46
</pre>

### Methodology

<pre>
1. Obtain back and front images of a clothing article (preferably with plain background).
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Apparels/tankTop.png" width="120" height="160">
2. Employ background subtraction thereby detecting and storing contours within the image.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/backgroundSubtraction.jpg" width="120" height="160"><img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/edgeDetection.jpg" width="120" height="160">
3. Extrapolate mesh points and perform delaunay triangulation on procured set of points.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/addPoints.jpg" width="120" height="160"><img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/test-1.0.PNG" width="150" height="160">
4. Align the object onto refernce model.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/alignment.PNG" width="200" height="200">
5. Carry out ray casting and cloth simulation.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/t1.PNG" width="150" height="150"><img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/t2.PNG" width="150" height="150"><img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/t3.PNG" width="112.5" height="150"><img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/t4.PNG" width="112.5" height="150">
</pre>

### Code Structure

#### ImageProcessor
<pre>
1. Loads back/front image.
2. Executes background subtraction.
3. Finds and stores contours.
</pre>

#### ModelGenerator
<pre>
1. Extrapolates mesh points within apparel boundary for both back and front parts.
2. Carries out delaunay triangulation on obtained points.
3. Carries out mesh-avatar intersection.
4. Integrates back and front models.
5. Stores model in obj format.
</pre>

#### Apparel
<pre>
  Initializes data structures for storing mesh components.
</pre>

#### Cloth Simulator
<pre>
1. Identifies and stores constraints viz structural, shear and flexion.
2. Performs adjustments wrt constraints to smooth-out the mesh.
</pre>

#### RayCaster
<pre>
  Checks ray-triangle intersection and returns intersection point.
</pre>
### Results
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/dresses.PNG" width="1083" height="578">
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/dresses1.PNG" width="1083" height="578">
