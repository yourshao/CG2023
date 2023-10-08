#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>


#define WIDTH 320
#define HEIGHT 240

//for grey spectrum

std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues){
    std::vector<float> result;
    float difference = ( to - from ) /( numberOfValues-1 )  ;
    for (int a = 0 ;a < numberOfValues; a++){
        result.push_back(from + a * difference);
    }
    return result;
}


std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues){
std::vector<glm::vec3> result (numberOfValues)  ;
    float oneFrom;
    float oneTo;
    for(int a = 0; a<3;a++) {
        oneFrom = from[a];
        oneTo = to[a];
        std::vector<float> oneVectorResult = interpolateSingleFloats(oneFrom, oneTo, numberOfValues);

        if (a == 0 ){
            for(int x = 0; x< numberOfValues; x++){
                result[x].x =oneVectorResult[x];
            }
        }else if (a == 1 ){
            for(int x = 0; x< numberOfValues; x++){
                result[x].y =oneVectorResult[x];
            }
        } else if (a == 2 ){
            for(int x = 0; x< numberOfValues; x++){
                result[x].z =oneVectorResult[x];
            }
        }

    }

    return result;

}

void Drawanother(float fromX, float fromY, float toX, float toY, DrawingWindow &window){
    float xDiff = toX - fromX;
    float yDiff = toY - fromY;
    float numberOfSteps = fmax(abs(xDiff), abs(yDiff));
    float xStepSize =  xDiff/numberOfSteps;
    float yStepSize =  yDiff/numberOfSteps;
    for (float i = 0.0; i <= numberOfSteps; i++) {
        float x = fromX + (xStepSize*i);
        float y = fromY + (yStepSize*i);
        uint32_t colour = (255 << 24) + (255 << 16) + (255 << 8) + 255;

        window.setPixelColour(round(x), round(y), colour);
    }
}







void draw(DrawingWindow &window) {
	window.clearPixels();


//    CanvasPoint::CanvasPoint picture;
//    int red;
//    int green;
//    int blue;
//
//
//
//    for (size_t y = 0; y < window.height; y++) {
//        // for grey spectrum
////        std::vector<float> result;
////        result = interpolateSingleFloats(0, 255, WIDTH);
//       for (size_t x = 0; x < window.width; x++) {
//
//
//           if (picture.brightness(x,y) ==1.0 ){
//               red = 0;
//               green = 0;
//               blue = 0;
//           }else{
//               red =255;
//               green = 255;
//               blue =255;
//           }
//            uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
//			window.setPixelColour(x, y, colour);
//		}
//	}



    Drawanother(0.0, 0.0, WIDTH/2, HEIGHT/2,window);
    Drawanother(WIDTH/3, HEIGHT/2, 2*WIDTH/3, HEIGHT/2,window);
    Drawanother(WIDTH, 0.0,WIDTH/2, HEIGHT/2, window);
    Drawanother(WIDTH/2, 0,WIDTH/2, HEIGHT, window );




}






void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}




int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;






    while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}



}
