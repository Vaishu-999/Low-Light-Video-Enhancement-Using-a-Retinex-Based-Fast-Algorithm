#include<iostream>
#include <opencv2/opencv.hpp>
#include<chrono>
using namespace std::chrono;
using namespace std;
using namespace cv;

Mat readImage(string imagePath)
{

    Mat image= imread(imagePath, cv::IMREAD_COLOR);
    return image;
}
std::string getTypeString(int type) {
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "Unknown"; break;
    }

    r += "C" + std::to_string(chans);  // Append number of channels
    return r;
}


int main()
{
    //To supress verbose logs   
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);

    // Start overall timing
    auto overallStart = high_resolution_clock::now();

    //read image
    string imagePath = "INPUTS/input10.jpg";


    //Mat is a object of opencv, simply short cut for Matrix. Mat speicfically designed for image operations
    // and it will organize 3 channels correctly.
    Mat bgrImg = readImage(imagePath);
    cout<<"bgrImg image data type : " << getTypeString(bgrImg.type());


    //check if image is loaded successfully
    if( bgrImg.empty()){
        cout<<imagePath<<"not found "<< endl;
        return -1;
    }


    //printing of the rows
    cout << "Rows = " << bgrImg.rows << ", Cols =  " << bgrImg.cols << ", channels = " << bgrImg.channels() << ", Depth = " << bgrImg.depth()<< endl;
    cout << "Type of image.rows: " << typeid(bgrImg.rows).name() << endl;


    //convert BGR color image to grayscale image
    Mat grayImg;
    cvtColor(bgrImg, grayImg, COLOR_BGR2GRAY);


    //extracting rows and coloumns from grayimage(it should be int cause resouvtion of the image will always be integere dimension)
    int rows = grayImg.rows;
    int cols = grayImg.cols;


    //convert BGR color space to HSV color space
    Mat hsvImg;
    cvtColor(bgrImg, hsvImg, COLOR_BGR2HSV);



    //split the channels
    vector<Mat> channels;
    split(hsvImg, channels);
    Mat hChannel = channels[0];
    Mat sChannel = channels[1];
    Mat vChannel = channels[2];


    //.................................................................................................................................
    //%finding gray and s channel frequencies-----------------------------------

    int histSize = 256;  //no.of bins
    float range[] = {0,256}; // why I specify float here? becuase calcHist function needs values to be in float
    const float* histRange= { range }; 
    bool uniform = true;
    bool accumulate = false;

    Mat vGrayFreq,sGrayFreq;

    // Calculate histogram for V channel
    calcHist(&grayImg, 1, 0, Mat(), vGrayFreq, 1, &histSize, &histRange, uniform, accumulate);

    // Calculate histogram for S channel
    calcHist(&sChannel, 1, 0, Mat(), sGrayFreq, 1, &histSize, &histRange, uniform, accumulate);

    //.........................................................................................................................

    // #To get the maximum Gray level Value (that means not the gray value which has highest frequency , it means the maximum gravalue which has non-zer0 )from V channel of HSV Image
    //that is why we can't use any direct maximum functions here.
    int vMaxVal;
     for (int i = 255; i >= 0 ; --i)
     {
        if(vGrayFreq.at<float>(i) != 0)
        {
            vMaxVal= i;
            break;
        }
     }
     cout<<"\nvMax value : "<<vMaxVal;



    //................................To calcualte gamma preparing prerequisites ...................................................

    //calculation of mean.....
    //Here I cannot use mean function as well....
    //here you can use direct mean function if you want to mean direct vChannel, but here you are finding mean using vGrayFreq, so you have to use manual mean.

    double sum = 0.0, totalPixels = 0.0;

    for (int i = 0; i<256 ; i++)
    {
        float frequency = vGrayFreq.at<float>(i);
        sum = sum + (i * frequency);
        totalPixels = totalPixels + frequency;
    }

    int meann = int(sum / totalPixels) ;//here previously used mean variable as double but I changed to int 
    cout<< "mean value : "<< meann;


    //calculating constant c .....
    double temp1 = 0.0;
    double temp2 = 0.0;
    for( int i = 0; i<=meann; ++i)
    {
        float freq = vGrayFreq.at<float>(i);
        temp1 = temp1 + (freq * i);
        temp2 =temp2 + freq;
    }

    double c = temp1 / (128.0 * temp2);
    cout<< "\n constant c : "<< c <<"\n";
    cout<<"are you working ";



    //Enlarging the constant c .....
    double c1 = 1.0 / (1.0 + exp(-c));
    float enlargedC = floor(c1);
    cout<<"double c : "<< c1 <<endl;
    cout<<"float c : "<<enlargedC <<endl;

    
    //calculate pdf and cdf .....
    int resolution = rows * cols;
    double cdfVal = 0.0;

    for (int i = 0; i <= 128 ; ++i)
    {
        float freq = vGrayFreq.at<float>(i);
        double pdf = freq / resolution;
        cdfVal = cdfVal + pdf;
    }

    cout << "cdf value is : " << cdfVal <<endl;

    //calculate gamma ..............

    double w = 0.48;
    double gamma = w * enlargedC + (1.0 - w ) * cdfVal;

    cout<< "gamma : " << gamma;

    //..............................Ingridients ready to do Vchannel enhancement .............................

    //1.linear enhancement......
    double temp = 255.0 / vMaxVal;
    Mat enhancedVChannelImg;
    enhancedVChannelImg = vChannel.clone();
    cout<<"vChannel type in linear enhancement : "<< getTypeString(vChannel.type()) <<endl;

    //Vchannel enhnacing......................................................................................................................
    //src.convertTo(dst, new_type, alpha, beta);
    //Above function  applies below  formula to each pixel:
    //dst(x, y) = src(x, y) * alpha + beta

    //gamma applying

    Mat vGammaCorrected0To1;
    Mat vNormalizeTemp;
    enhancedVChannelImg.convertTo(vNormalizeTemp, CV_32F , 1.0/255.0);

        //apply gamma correction vGammaCorrected = vNormalizeTemp ^ gamma
    cv::pow(vNormalizeTemp, gamma ,vGammaCorrected0To1 ); //vGammaCorrected still in normalized form only, so we have to convert back to 0-255
    Mat vGammaCorrected; //vGammaCorrected 0-255
    vGammaCorrected0To1.convertTo(vGammaCorrected, CV_8U , 255.0);
    // Log before converting back
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            float oldVal = enhancedVChannelImg.at<uchar>(i, j);
            float norm = vNormalizeTemp.at<float>(i, j);
            uchar powVal = vGammaCorrected.at<uchar>(i, j);
            //float finalVal = powVal * 255.0;
            cout << "[C++] Pixel (" << i << "," << j << "): old=" << oldVal << ", norm=" << norm << ", pow=" << static_cast<int>(powVal) << ", gamma=" << gamma << ", final=" << static_cast<int>(powVal) << endl;
        }
    }
    //Replace the original enhancedVChannelImg
    enhancedVChannelImg = vGammaCorrected.clone();
    //#VE = exp(log(V) - log(VL) .........................
    cout<< endl <<"....................................Start VE..................................."<<endl;
    for (int i = 0 ; i < rows ; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            float change1 = enhancedVChannelImg.at<uchar>(i,j) / 255.0f;
            if (i < 5 && j < 5)
                cout<<" change 1 : "<< change1 ;
            float change2 = vChannel.at<uchar>(i,j) / 255.0f;
            if(i < 5 && j < 5)
                cout<<" , change2 : "<< change2 <<endl;
            //applyting log if non-zero
            if(change1 != 0.0f)
            {
                change1 = cv::log(change1);
            }
            if(change2 != 0.0f)
            {
                change2 = cv::log(change2);
            }
            float tresult = 0.0f ;
            if(change1 != 0.0f)
            {
                tresult = (exp(change2 - change1));
            }
            int result = int(255*tresult);
            if((i>870 && i<881) && (j>1310 && j<1321))
                cout<<endl<<"enhanced v after apply chnage2 -change1 : "<<result;


            //Dynamic Range expansion 

            if (result < 120 )
            {
                float drTemp;
                float fresult1 = float(result);
                fresult1 = fresult1 / 255.0f ;
                drTemp = 2.5f * fresult1 * fresult1;
                drTemp = drTemp * 255.0f ;

                int fresult2 = static_cast<int>(floor(drTemp));// here floor why ? 
                if((i>2 && i<5) && (j>2 && j<5))
                    cout << "[C++] Pixel (" << i << "," << j << "): After Dynamic range=" << fresult2  << endl;
                enhancedVChannelImg.at<uchar>(i,j) = static_cast<uchar>(fresult2);
                if((i>2 && i<5) && (j>2 && j<5))
                    cout << "[C++] Pixel (" << i << "," << j << "): enhanced=" << static_cast<int>(enhancedVChannelImg.at<uchar>(i,j)) << endl;
            }

        }
    }
    cout<< "After Dynamic range expansion : "<<getTypeString(enhancedVChannelImg.type())<<endl;

    cout<< endl <<".................................................End VE........................................................"<<endl;



    //calculation of Smean Vmean and VES
    double veMean = cv::mean(enhancedVChannelImg)[0];
    cout << "LOG VE mean : " << veMean <<endl;

    double sMean = 0.0;
    for (int i = 0; i < 256 ; ++i)
    {
        sMean = sMean + sGrayFreq.at<float>(i)*i;
    }
    sMean = sMean / (rows * cols);
    cout << "[LOG] S Mean (from histogram): " << sMean << endl;
    double sMeantemp = cv::mean(sChannel)[0];
    cout << "s mean using mean function : "<< sMeantemp <<endl;
    double ves = veMean - sMean;
    cout << "[LOG] VES (VE mean - S mean): " << ves << endl;

    
    
    // Saturation adjustment ..................................................................................................................
    //I compute this calculation with the manula loop as well. But output is same. The main problem lies somewhere in VE mean calculation. But below part is correct.
    cout<<"...................................................start Saturation Adjustment................................................................"<<endl;
    int n = (ves >= 0) ? 1 : 0;
    Mat enhancedSChannelImg;
    sChannel.convertTo(enhancedSChannelImg, CV_32F , 1.0/255.0);
    cout<< "schannel data type after conversion : "<<getTypeString(enhancedSChannelImg.type())<<endl;
    cout<<" enhancedSChannelImg 0 to 1 value " << enhancedSChannelImg.at<float>(2,2)<<endl;

    ves = ves/255.0;
    float sign = cv::pow(-1, 2 - n);
    float absVES = cv::abs(ves);
    float tempPow = 1.0 + sign * (absVES * absVES + absVES);
    cout<<"tempPow : " << tempPow;
    Mat tempsChannel = enhancedSChannelImg.clone();
    cv::pow(enhancedSChannelImg, tempPow ,tempsChannel );
    cout<< "schannel data type after conversion : "<<getTypeString(tempsChannel.type())<<endl;
    cout<<" enhancedSChannelImg 0 to 1 value " << tempsChannel.at<float>(2,2)<<endl;
    tempsChannel.convertTo(enhancedSChannelImg, CV_8U , 255.0);
    cout<<" enhancedSChannelImg 255 value " << static_cast<int>(enhancedSChannelImg.at<uchar>(2,2))<<endl;
    cout<<"...................................................end Saturation Adjustment................................................................"<<endl;


    vector<Mat> updatedChannels = { hChannel, enhancedSChannelImg , enhancedVChannelImg };
    Mat updatedHSV;
    cv::merge(updatedChannels, updatedHSV);

    // Convert HSV to BGR
    Mat outputImage;
    cv::cvtColor(updatedHSV, outputImage, COLOR_HSV2BGR);

    auto overallEnd = high_resolution_clock::now();
    auto totalDuration = duration_cast<milliseconds>(overallEnd - overallStart);
    cout << "[TIMER] Total Time from Start to End: " << totalDuration.count() / 1000.0 << " seconds" << endl;

    imshow("Display window", outputImage);

    waitKey(0);
    return 0;
}
/*
Optimization options
1. In Vchannel enhancement you used 2 for loops one for gamma applying second for VE. So you can put both in one loop
2. Bad ouput is not because of using direct inbuilt functions for sChannel, It is because of VE channel. make sure to write correctly
3. Be careful with conversions


*/