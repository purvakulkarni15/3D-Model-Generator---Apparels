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
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Articles/blob/main/Result/Sample-1-f.png" width="210" height="260">
2. Employ background subtraction followed by edge detection.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Articles/blob/main/Result/edgeDetection.jpg" width="210" height="260">
3. Extrapolate mesh points and perform delaunay triangulation on procured set of points to create a 2D object.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/addPoints.jpg" width="250" height="260"><img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/test-1.0.PNG" width="250" height="260">
4. Obtain stitching points from user.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Articles/blob/main/Result/stitchPoints.PNG" width="250" height="260">
5. Align the object onto reference avatar.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Articles/blob/main/Result/alignment.PNG" width="250" height="260">
6. Carry out object collision using ray casting followed by cloth constraint satisfaction to conserve inter particle distance.
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Articles/blob/main/Result/Sample-1-3d.PNG" width="250" height="260">
</pre>

### Results
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/dresses.PNG" width="1083" height="578">
<img src="https://github.com/purvakulkarni15/Rapid-Digitization-Of-Apparels/blob/master/Result/dresses1.PNG" width="1083" height="578">
