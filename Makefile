all:
	g++ main.cpp ./sceneObjectRenderers/*.cpp -o main -g \
	-lglfw -lX11 -ldl -lXrandr -lGLEW -lGL -DGL_SILENCE_DEPRECATION -DGLM_ENABLE_EXPERIMENTAL -I. 