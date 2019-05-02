#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
using namespace std;

struct Pixel
{
    unsigned char greenVal;
    unsigned char redVal;
    unsigned char blueVal;

};

struct Header
{

    vector<Pixel> vecPix;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
    char idLength;
    char colourMapType;
    char dataTypeCode;
    char colourMapOrigin;
    char colourMapLength;
    char colourMapDepth;


};



vector<Pixel> AddGreen(vector<Pixel> vecPix);
vector<Pixel> ScaleRedAndBlue(vector<Pixel> vecPix);
vector<Pixel> makeOnly(vector<Pixel> vecPix, string channel);
vector<Pixel> Combine(vector<Pixel> blueHead, vector<Pixel> greenHead, vector<Pixel> redHead);
vector<Pixel> Torque(vector<Pixel> vecPix);
void writeTo(const char* fileName, Header headerObject, vector<Pixel> vecPix);
bool test(const char* filename, const char* secondFile, int casenumber);
bool getTest(const char* filename, const char* secondFile, int casenumber);
vector<Pixel> Multiply(vector<Pixel> layer_One, vector<Pixel> layer_Two, int size);
Header makeHeader(const char* fileName, Header &headerObject);
vector<Pixel> Subtract(vector<Pixel> layer_One, vector<Pixel> layer_Two, int size);
vector<Pixel> Screen(vector<Pixel> layer_One, vector<Pixel> layer_Two, int size);
vector<Pixel> Overlay(vector<Pixel> layer_One, vector<Pixel> layer_Two, int size);


vector<Pixel> Multiply(vector<Pixel> layer_One, vector<Pixel> layer_Two, int size) {
    vector<Pixel > newMultiple;

    for (int i = 0; i < size; i++) {
        Pixel ray;
        float blue_One = (float)layer_One[i].blueVal / 255;
        //  blue_One += 0.5f;
        float green_One = (float)layer_One[i].greenVal / 255;
        //  green_One += 0.5f;
        float red_One = (float)layer_One[i].redVal / 255;
        //  red_One += 0.5f;
        float blue_Two = (float)layer_Two[i].blueVal / 255;
        // blue_Two += 0.5f;
        float green_Two = (float)layer_Two[i].greenVal / 255;
        // green_Two += 0.5f;
        float red_Two = (float)layer_Two[i].redVal / 255;
        // red_Two += 0.5f;
        ray.blueVal = (blue_One * blue_Two) * 255 + 0.5;
        ray.greenVal = (green_One * green_Two) * 255 + 0.5;
        ray.redVal = (red_One * red_Two) * 255 + 0.5;

        newMultiple.push_back(ray);
    }
    return newMultiple;
}

vector<Pixel > Subtract(vector<Pixel > layer_One, vector<Pixel > layer_Two, int size) {
    vector<Pixel > newSubtraction;
    for (int i = 0; i < size; i++) {
        Pixel faze;
        faze.blueVal = max(layer_One[i].blueVal - layer_Two[i].blueVal, 0);
        faze.greenVal = max(layer_One[i].greenVal - layer_Two[i].greenVal, 0);
        faze.redVal = max(layer_One[i].redVal - layer_Two[i].redVal, 0);

        newSubtraction.push_back(faze);
    }

    return newSubtraction;
}

vector<Pixel > Screen(vector<Pixel > layer_One, vector<Pixel > layer_Two, int size) {
    vector<Pixel > newScreen;
    for (int i = 0; i < size; i++) {
        Pixel rave;
        float blue_One = layer_One[i].blueVal / 255.0f;
        float green_One = layer_One[i].greenVal / 255.0f;
        float red_One = layer_One[i].redVal / 255.0f;
        float blue_Two = layer_Two[i].blueVal / 255.0f;
        float green_Two = layer_Two[i].greenVal / 255.0f;
        float red_Two = layer_Two[i].redVal / 255.0f;

        rave.blueVal = (1 -(1- blue_One) * (1 -blue_Two)) * 255 + 0.5;
        rave.greenVal = (1- (1 - green_One) * (1 - green_Two)) * 255 + 0.5;
        rave.redVal = (1 - (1- red_One) * (1 - red_Two)) * 255 + 0.5;

        newScreen.push_back(rave);
    }

    return newScreen;
}

vector<Pixel> Overlay(vector<Pixel> layer_One, vector<Pixel> layer_Two, int size) {
    vector<Pixel> newOverlay;

    for (int i = 0; i < size; i++) {
        Pixel rager;
        float blue_One = layer_One[i].blueVal / 255.0f;

        float green_One = layer_One[i].greenVal / 255.0f;

        float red_One = layer_One[i].redVal / 255.0f;

        float blue_Two = layer_Two[i].blueVal / 255.0f;

        float green_Two = layer_Two[i].greenVal / 255.0f;

        float red_Two = layer_Two[i].redVal / 255.0f;

        if (blue_Two <= 0.5f) {
            rager.blueVal = (2 * blue_One * blue_Two) * 255 + 0.5;
        }
        else {
            rager.blueVal = (1 - (2 * (1 - blue_One) * (1 - blue_Two))) * 255 + 0.5;
        }
        if (green_Two <= 0.5f) {
            rager.greenVal = (2 * green_One * green_Two) * 255 + 0.5;
        }
        else {
            rager.greenVal = (1 - (2 * (1 - green_One) * (1 - green_Two))) * 255 + 0.5;
        }
        if (red_Two <= 0.5f){
            rager.redVal = (2 * red_One * red_Two) * 255 + 0.5;
        }
        else {
            rager.redVal = (1 - (2 * (1 - red_One) * (1 - red_Two))) * 255 + 0.5;
        }

        newOverlay.push_back(rager);
    }

    return newOverlay;
}

Header makeHeader(const char* fileName, Header &headerObject) {
    ifstream file(fileName, ios_base::binary|ios_base::in);
    if (file.is_open())
    {
        file.read(&headerObject.idLength, 1);
        file.read(&headerObject.colourMapType, 1);
        file.read(&headerObject.dataTypeCode, 1);
        file.read(&headerObject.colourMapOrigin, 2);
        file.read(&headerObject.colourMapLength, 2);
        file.read(&headerObject.colourMapDepth, 1);
        file.read((char *)&headerObject.xOrigin, 2);
        file.read((char *)&headerObject.yOrigin, 2);
        file.read((char *)&headerObject.width, 2);
        file.read((char *)&headerObject.height, 2);
        file.read(&headerObject.bitsPerPixel, 1);
        file.read(&headerObject.imageDescriptor, 1);

        int imageDimensions = headerObject.width * headerObject.height;
        for (int i = 0; i < imageDimensions; i++) {
            Pixel smallGuy;
            file.read((char *)&smallGuy.blueVal, 1);
            file.read((char *)&smallGuy.greenVal, 1);
            file.read((char *)&smallGuy.redVal, 1);

            headerObject.vecPix.push_back(smallGuy);
        }

        file.close();
    }

    return headerObject;
}

void writeTo(const char* fileName, Header headerObject, vector<Pixel> vecPix) {
    ofstream newFile;
    newFile.open(fileName, ios_base::binary|ios_base::out);

    if (newFile.is_open()) {
        newFile.write(&headerObject.idLength, 1);
        newFile.write(&headerObject.colourMapType, 1);
        newFile.write(&headerObject.dataTypeCode, 1);
        newFile.write((char *)&headerObject.colourMapOrigin, 2);
        newFile.write((char *)&headerObject.colourMapLength, 2);
        newFile.write(&headerObject.colourMapDepth, 1);
        newFile.write((char *)&headerObject.xOrigin, 2);
        newFile.write((char *)&headerObject.yOrigin, 2);
        newFile.write((char *)&headerObject.width, 2);
        newFile.write((char *)&headerObject.height, 2);
        newFile.write(&headerObject.bitsPerPixel, 1);
        newFile.write(&headerObject.imageDescriptor, 1);

        int imageDimensions = headerObject.width * headerObject.height;
        for (int i = 0; i < imageDimensions; i++) {
            newFile.write((char *)&vecPix[i].blueVal, 1);
            newFile.write((char *)&vecPix[i].greenVal, 1);
            newFile.write((char *)&vecPix[i].redVal, 1);
        }

        newFile.close();
    }
}

vector<Pixel> AddGreen(vector<Pixel> vecPix) {
    vector<Pixel> newVector;

    for (unsigned int i = 0; i < vecPix.size(); i++) {
        Pixel newGuy;
        newGuy.blueVal = vecPix[i].blueVal;
        float place = vecPix[i].greenVal + 200.0f;
        if (place > 255) {
            place = 255;
        }
        newGuy.greenVal = place;
        newGuy.redVal = vecPix[i].redVal;

        newVector.push_back(newGuy);
    }
    return newVector;
}

vector<Pixel> ScaleRedAndBlue(vector<Pixel> vecPix) {
    vector<Pixel> newVector;

    for (unsigned int i = 0; i < vecPix.size(); i++) {
        Pixel newGuy;
        newGuy.blueVal = vecPix[i].blueVal * 0;
        newGuy.greenVal = vecPix[i].greenVal;
        float redPlace = vecPix[i].redVal * 4.0f;
        if(redPlace > 255) {
            redPlace = 255;
        }
        newGuy.redVal = redPlace;

        newVector.push_back(newGuy);
    }
    return newVector;
}

vector<Pixel> makeOnly(vector<Pixel> vecPix, string channel) {
    vector<Pixel> newVec;

    if (channel.compare("blue") == 0) {
        for (unsigned int i = 0; i < vecPix.size(); i++) {
            Pixel newGuy;
            newGuy.blueVal = vecPix[i].blueVal;
            newGuy.greenVal = vecPix[i].blueVal;
            newGuy.redVal = vecPix[i].blueVal;

            newVec.push_back(newGuy);
        }
    }
    else if (channel.compare("green") == 0) {
        for (unsigned int i = 0; i < vecPix.size(); i++) {
            Pixel newGuy;
            newGuy.blueVal = vecPix[i].greenVal;
            newGuy.greenVal = vecPix[i].greenVal;
            newGuy.redVal = vecPix[i].greenVal;

            newVec.push_back(newGuy);
        }
    }
    else if (channel.compare("red") == 0) {
        for (unsigned int i = 0; i < vecPix.size(); i++) {
            Pixel newGuy;
            newGuy.blueVal = vecPix[i].redVal;
            newGuy.greenVal = vecPix[i].redVal;
            newGuy.redVal = vecPix[i].redVal;

            newVec.push_back(newGuy);
        }
    }

    return newVec;
}

vector<Pixel> Combine(vector<Pixel> blueHead, vector<Pixel> greenHead, vector<Pixel> redHead) {
    vector<Pixel> newVec;
    int size = 600 * 398;
    for (int i = 0; i < size; i++) {
        Pixel newPix;
        float bluePlace = blueHead[i].blueVal / 255.0f;
        float greenPlace = greenHead[i].greenVal / 255.0f;
        float redPlace = redHead[i].redVal / 255.0f;

        newPix.blueVal = bluePlace * 255 + 0.5;
        newPix.greenVal = redPlace * 255 + 0.5;
        newPix.redVal = greenPlace * 255 + 0.5;

        newVec.push_back(newPix);
    }
    return newVec;
}

vector<Pixel> Torque(vector<Pixel> vecPix) {
    vector<Pixel> newVec;

    for (unsigned int i = 0; i < vecPix.size(); i++) {
        Pixel newPix;

        newPix.blueVal = vecPix[vecPix.size() - i - 1].blueVal;
        newPix.greenVal = vecPix[vecPix.size() - i - 1].greenVal;
        newPix.redVal = vecPix[vecPix.size() - i - 1].redVal;

        newVec.push_back(newPix);
    }

    return newVec;
}

bool test(const char* filename, const char* secondFile, int casenumber) {
    bool passed;
    Header testone;
    testone = makeHeader(filename, testone);
    Header testtwo;
    testtwo = makeHeader(secondFile, testtwo);
    for(int i = 0; i < testone.vecPix.size(); i++){
        if(testone.vecPix[i].blueVal != testtwo.vecPix[i].blueVal){
            passed = false;
        }
        else if(testone.vecPix[i].greenVal != testtwo.vecPix[i].greenVal){
            passed = false;
        }
        else if(testone.vecPix[i].redVal != testtwo.vecPix[i].redVal){
            passed = false;
        }
        else{
            continue;
        }

    }
    if(passed != true){
        cout << "Test #" << casenumber << " Failed!" << endl;
    }
    if (testone.width != testtwo.width){
        cout << "Test #" << casenumber << " Failed!" << endl;
    }
    else if(testone.bitsPerPixel != testtwo.bitsPerPixel){
        cout << "Test #" << casenumber << " Failed!" << endl;
    }
    else if(testone.imageDescriptor != testtwo.imageDescriptor){
        cout << "Test #" << casenumber << " Failed!" << endl;
    }
    else if(testone.colourMapDepth != testtwo.colourMapDepth){
        cout << "Test #" << casenumber << " Failed!" << endl;
    }
    else if(testone.xOrigin != testtwo.xOrigin){
        cout << "Test #" << casenumber << " Failed!" << endl;
    }
    else if(testone.colourMapLength != testtwo.colourMapLength){
        cout << "Test #" << casenumber << " Failed!"<< endl;
    }
    else if(testone.colourMapOrigin != testtwo.colourMapOrigin){
        cout << "Test #" << casenumber << " Failed!"<< endl;
    }
    else if(testone.colourMapType != testtwo.colourMapType){
        cout << "Test #" << casenumber << " Failed!"<< endl;
    }
    else if(testone.dataTypeCode != testtwo.dataTypeCode){
        cout << "Test #" << casenumber << " Failed!"<< endl;
    }
    else if(testone.height != testtwo.height){
        cout << "Test #" << casenumber << " Failed!"<< endl;
    }
    else if(testone.idLength != testtwo.idLength){
        cout << "Test #" << casenumber << " Failed!"<< endl;
    }
    else if(testone.yOrigin != testtwo.yOrigin){
        cout << "Test #" << casenumber << " Failed!"<< endl;
    }
    else{
        cout << "Test #" << casenumber << " Passed!"<< endl;
        passed = true;
    }

    return passed;

}
bool getTest(const char* filename, const char* secondFile, int casenumber){
    bool passed = true;
    Header testone;
    testone = makeHeader(filename, testone);
    Header testtwo;
    testtwo = makeHeader(secondFile, testtwo);
    for(int i = 0; i < testone.vecPix.size(); i++){
        if(testone.vecPix[i].blueVal != testtwo.vecPix[i].blueVal){
            passed = false;
        }
        else if(testone.vecPix[i].greenVal != testtwo.vecPix[i].greenVal){
            passed = false;
        }
        else if(testone.vecPix[i].redVal != testtwo.vecPix[i].redVal){
            passed = false;
        }
        else{
            continue;
        }
    }
    if (testone.width != testtwo.width){
        passed = false;
    }
    else if(testone.bitsPerPixel != testtwo.bitsPerPixel){
        passed = false;
    }
    else if(testone.imageDescriptor != testtwo.imageDescriptor){
        passed = false;
    }
    else if(testone.colourMapDepth != testtwo.colourMapDepth){
        passed = false;
    }
    else if(testone.xOrigin != testtwo.xOrigin){
        passed = false;
    }
    else if(testone.colourMapLength != testtwo.colourMapLength){
        passed = false;
    }
    else if(testone.colourMapOrigin != testtwo.colourMapOrigin){
        passed = false;
    }
    else if(testone.colourMapType != testtwo.colourMapType){
        passed = false;
    }
    else if(testone.dataTypeCode != testtwo.dataTypeCode){
        passed = false;
    }
    else if(testone.height != testtwo.height){
        passed = false;
    }
    else if(testone.idLength != testtwo.idLength){
        passed = false;
    }
    else if(testone.yOrigin != testtwo.yOrigin){
        passed = false;
    }
    else{
        passed = true;
    }

    return passed;

}



int main() {
    //Reading layer1.tga file
    Header headerObject;
    headerObject = makeHeader("input/layer1.tga", headerObject);

    //Reading layer2 file
    Header headerObjTwo;
    headerObjTwo = makeHeader("input/layer2.tga", headerObjTwo);


    // Reading pattern1.tga
    Header pattOne;
    pattOne = makeHeader("input/pattern1.tga", pattOne);


    // Reading pattern2.tga
    Header pattTwo;
    pattTwo = makeHeader("input/pattern2.tga", pattTwo);

    // Reading text.tga
    Header textOne;
    textOne = makeHeader("input/text.tga", textOne);

    // Reading text2.tga
    Header textTwo;
    textTwo = makeHeader("input/text2.tga", textTwo);

    // Reading car.tga
    Header carHead;
    carHead = makeHeader("input/car.tga", carHead);

    // Reading circles.tga
    Header circleHead;
    circleHead = makeHeader("input/circles.tga", circleHead);


    // Reading layer_blue.tga
    Header blueHead;
    blueHead = makeHeader("input/layer_blue.tga", blueHead);

    // Reading layer_green.tga
    Header greenHead;
    greenHead = makeHeader("input/layer_green.tga", greenHead);

    // Reading layer_red.tga
    Header redHead;
    redHead = makeHeader("input/layer_red.tga", redHead);

    // Task 1
    vector<Pixel> multiplied_One = Multiply(headerObject.vecPix, pattOne.vecPix, (headerObject.width * headerObject.height));
    writeTo("output/part1.tga", headerObject, multiplied_One);
    test("examples/EXAMPLE_part1.tga", "output/part1.tga", 1);

    // Task 2
    vector<Pixel> subtract_One = Subtract(carHead.vecPix, headerObjTwo.vecPix, (headerObjTwo.width * headerObjTwo.height));
    writeTo("output/part2.tga", headerObjTwo, subtract_One);
    test("examples/EXAMPLE_part2.tga", "output/part2.tga", 2);

    // Task 3
    vector<Pixel> temp = Multiply(headerObject.vecPix, pattTwo.vecPix, (headerObject.width * headerObject.height));
    vector<Pixel> screen_One = Screen(textOne.vecPix, temp, (textOne.width * textOne.height));
    writeTo("output/part3.tga", textOne, screen_One);
    test("examples/EXAMPLE_part3.tga", "output/part3.tga", 3);

    // Task 4
    vector<Pixel> multiply_Temp = Multiply(circleHead.vecPix, headerObjTwo.vecPix, (headerObjTwo.width * headerObjTwo.height));
    vector<Pixel> subtract_Two = Subtract(multiply_Temp, pattTwo.vecPix, (pattTwo.width * pattTwo.height));
    writeTo("output/part4.tga", pattTwo, subtract_Two);
    test("examples/EXAMPLE_part4.tga", "output/part4.tga", 4);

    // Task 5
    vector<Pixel> overlay_One = Overlay(headerObject.vecPix, pattOne.vecPix, (headerObject.width * headerObject.height));
    writeTo("output/part5.tga", headerObject, overlay_One);
    test("examples/EXAMPLE_part5.tga", "output/part5.tga", 5);


    // Task 6
    vector<Pixel> newGreens = AddGreen(carHead.vecPix);
    writeTo("output/part6.tga", carHead, newGreens);
    test("examples/EXAMPLE_part6.tga", "output/part6.tga", 6);

    // Task 7
    vector<Pixel> newRedAndBlue = ScaleRedAndBlue(carHead.vecPix);
    writeTo("output/part7.tga", carHead, newRedAndBlue);
    test("examples/EXAMPLE_part7.tga", "output/part7.tga", 7);

    // Task 8
    vector<Pixel> blueOnly = makeOnly(carHead.vecPix, "blue");
    vector<Pixel> greenOnly = makeOnly(carHead.vecPix, "green");
    vector<Pixel> redsOnly  = makeOnly(carHead.vecPix, "red");
    writeTo("output/part8_b.tga", carHead, blueOnly);
    writeTo("output/part8_g.tga", carHead, greenOnly);
    writeTo("output/part8_r.tga", carHead, redsOnly);
    test("examples/EXAMPLE_part8_b.tga", "output/part8_b.tga", 8);
    test("examples/EXAMPLE_part8_g.tga", "output/part8_g.tga", 9);
    test("examples/EXAMPLE_part8_r.tga", "output/part8_r.tga", 10);

    // Task 9
    vector<Pixel> combined = Combine(blueHead.vecPix, redHead.vecPix, greenHead.vecPix);
    Header combinedHead;
    blueHead.width = 600;
    blueHead.height = 398;
    writeTo("output/part9.tga", blueHead, combined);
    test("examples/EXAMPLE_part9.tga", "output/part9.tga", 11);


    // Task 10
    vector<Pixel> rotated = Torque(textTwo.vecPix);
    writeTo("output/part10.tga", textTwo, rotated);
    test("examples/EXAMPLE_part10.tga", "output/part10.tga", 12);

    double score = 0;
    if( getTest("examples/EXAMPLE_part1.tga", "output/part1.tga", 1) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part2.tga", "output/part2.tga", 2) == true){
        score = score + 11.25;
    }
    if(getTest("examples/EXAMPLE_part3.tga", "output/part3.tga", 3) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part4.tga", "output/part4.tga", 4) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part5.tga", "output/part5.tga", 5) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part6.tga", "output/part6.tga", 6) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part7.tga", "output/part7.tga", 7) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part8_b.tga", "output/part8_b.tga", 8) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part8_g.tga", "output/part8_g.tga", 9) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part8_r.tga", "output/part8_r.tga", 10) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part10.tga", "output/part10.tga", 11) == true){
        score = score + 11.25;
    }
    if( getTest("examples/EXAMPLE_part10.tga", "output/part10.tga", 12) == true){
        score = score + 11.25;
    }

    cout << "Total score "<< score << "/135 " <<endl;


    return 0;
}
