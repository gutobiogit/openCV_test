//import opencv and IO
#include<opencv2/opencv.hpp>
#include<iostream>

//set namespaces for library
using namespace std;
using namespace cv;

//set global variables
string path_image;
Mat original_image, result_image, dst;

//class read/write virtual
class In_Out
{
    public:
        void Load()
        {
            //try read file
            original_image = imread(path_image, IMREAD_UNCHANGED);
        }

        void Save()
        {
            //save file in program path
            imwrite("result_image" + path_image.substr(path_image.find(".")), dst);
        }
        
        //set virtual class
        virtual void transformation() = 0;
};

//class child In_out, grayscale
class Grayscale : public In_Out
{
    public:
        void transformation()
        {
            //transform image in grayshade
            cvtColor(original_image, result_image, COLOR_BGR2GRAY);
        }
};

//class child In_out, rotate
class Rotate : public In_Out
{
    public:
        void transformation()
        {
            // set rotate image angle
            double angle = -45;

            //get rotation matrix for rotating the image around its center in pixel coordinates
            Point2f center((result_image.cols - 1) / 2.0, (result_image.rows - 1) / 2.0);
            Mat rot = getRotationMatrix2D(center, angle, 1.0);
            //determine bounding rectangle, center not relevant
            Rect2f bbox = cv::RotatedRect(Point2f(), result_image.size(), angle).boundingRect2f();
            //adjust transformation matrix
            rot.at<double>(0, 2) += bbox.width / 2.0 - result_image.cols / 2.0;
            rot.at<double>(1, 2) += bbox.height / 2.0 - result_image.rows / 2.0;
            warpAffine(result_image, dst, rot, bbox.size());
        }
};

 int main()
{
    //input variable path_image
    cout << "Enter the image location:" << endl;
    cin >> path_image;
     
    //call Load method, load file
    In_Out* load_file;
    Grayscale object;
    load_file = &object;
    load_file->Load();

     //check if file exist or extension is acceptable
    if (!original_image.data)
    {
        cout << "FILE NOT FOUND, NOT COLOR IMAGE, EXTENSION NOT ALLOWED (.bmp, .dib, .jpeg, .jpg, .jpe, .jp2, .png, .webb, .pbm, .pgm, .ppm, .pxm, .pnm, .pfm, .sr, .ras, .hff, .tif, .exr, .hdr, .pic)" << endl << endl;
        return main();
    };

    //call class grayscale
    In_Out* grayscale= new Grayscale();
    grayscale->transformation();

    //call class rotate
    In_Out* rotate = new Rotate();
    rotate->transformation();

    //call Save method, save file
    In_Out* save_file;
    Rotate object2;
    save_file = &object2;
    save_file->Save();

    //set position and show original image
    imshow("Original Image",original_image);
    moveWindow("Original Image", 50, 20);

    //set position and show converted image
    imshow("Result Image", dst);
    moveWindow("Result Image", original_image.size().width + 50, 20);

    //wait for a key and destroy all windows, end of program with return 0
    waitKey(0);
    destroyAllWindows();
    return 0;
}