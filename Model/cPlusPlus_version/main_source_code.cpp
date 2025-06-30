#include<iostream>
#include<opencv2/opencv.hpp>
#include<chrono>

using namespace std;
using namespace std::chrono;
using namespace cv;

Mat readImage(const string& imagePath) {
    return imread(imagePath, IMREAD_COLOR);
}

string getTypeString(int type) {
    string r;
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
    r += "C" + to_string(chans);
    return r;
}

void calculateHistogram(const Mat& image, Mat& histogram) {
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    calcHist(&image, 1, 0, Mat(), histogram, 1, &histSize, &histRange, true, false);
}

int findMaxGrayLevel(const Mat& histogram) {
    for (int i = 255; i >= 0; --i) {
        if (histogram.at<float>(i) != 0) return i;
    }
    return -1;
}

int calculateMean(const Mat& histogram) {
    double sum = 0.0, totalPixels = 0.0;
    for (int i = 0; i < 256; i++) {
        float freq = histogram.at<float>(i);
        sum += i * freq;
        totalPixels += freq;
    }
    return int(sum / totalPixels);
}

double calculateConstantC(const Mat& histogram, int mean) {
    double temp1 = 0.0, temp2 = 0.0;
    for (int i = 0; i <= mean; ++i) {
        float freq = histogram.at<float>(i);
        temp1 += freq * i;
        temp2 += freq;
    }
    return temp1 / (128.0 * temp2);
}

double calculateCDF(const Mat& histogram, int threshold, int totalPixels) {
    double cdfVal = 0.0;
    for (int i = 0; i <= threshold; ++i) {
        cdfVal += histogram.at<float>(i) / totalPixels;
    }
    return cdfVal;
}

double calculateGamma(double c, double cdfVal, double w = 0.48) {
    double c1 = 1.0 / (1.0 + exp(-c));
    float enlargedC = floor(c1);
    return w * enlargedC + (1.0 - w) * cdfVal;
}

void applyGammaCorrection(Mat& channel, double gamma) {
    Mat normFloat, gammaCorrected;
    channel.convertTo(normFloat, CV_32F, 1.0 / 255.0);
    pow(normFloat, gamma, normFloat);
    normFloat.convertTo(gammaCorrected, CV_8U, 255.0);
    channel = gammaCorrected.clone();
}

void applyVE(Mat& enhancedV, const Mat& originalV) {
    int rows = enhancedV.rows;
    int cols = enhancedV.cols;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float ve1 = enhancedV.at<uchar>(i, j) / 255.0f;
            float ve2 = originalV.at<uchar>(i, j) / 255.0f;
            if (ve1 != 0.0f) ve1 = log(ve1);
            if (ve2 != 0.0f) ve2 = log(ve2);
            float result = (ve1 != 0.0f) ? exp(ve2 - ve1) : 0.0f;
            int finalVal = static_cast<int>(255 * result);

            if (finalVal < 100) {
                float fVal = finalVal / 255.0f;
                fVal = 2.5f * fVal * fVal * 255.0f;
                finalVal = static_cast<int>(floor(fVal));
            }
            enhancedV.at<uchar>(i, j) = static_cast<uchar>(finalVal);
        }
    }
}

void adjustSaturation(Mat& sChannel, double ves) {
    int n = (ves >= 0) ? 1 : 0;
    Mat floatS;
    sChannel.convertTo(floatS, CV_32F, 1.0 / 255.0);

    for (int i = 0; i < floatS.rows; ++i) {
        for (int j = 0; j < floatS.cols; ++j) {
            float val = floatS.at<float>(i, j);
            float vesNorm = ves / 255.0f;
            float sign = pow(-1, 2 - n);
            float absVES = abs(vesNorm);
            float tempPow = 1.0f + sign * (absVES * absVES + absVES);
            floatS.at<float>(i, j) = pow(val, tempPow);
        }
    }

    floatS.convertTo(sChannel, CV_8U, 255.0);
}

int main() {
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_ERROR);
    auto overallStart = high_resolution_clock::now();

    string imagePath = "C:/Users/vaish/Desktop/Motorola_Hacakthon/projects/c++/RBFA_project/device_input_frames/v200_input_frames/stairs/frame_00001.png";
    Mat bgrImg = readImage(imagePath);
    if (bgrImg.empty()) {
        cout << imagePath << " not found" << endl;
        return -1;
    }

    Mat grayImg;
    cvtColor(bgrImg, grayImg, COLOR_BGR2GRAY);

    Mat hsvImg;
    cvtColor(bgrImg, hsvImg, COLOR_BGR2HSV);

    vector<Mat> channels;
    split(hsvImg, channels);
    Mat hChannel = channels[0], sChannel = channels[1], vChannel = channels[2];

    Mat vHist, sHist;
    calculateHistogram(grayImg, vHist);
    calculateHistogram(sChannel, sHist);

    int vMax = findMaxGrayLevel(vHist);
    int mean = calculateMean(vHist);
    double c = calculateConstantC(vHist, mean);

    int totalPixels = grayImg.rows * grayImg.cols;
    double cdfVal = calculateCDF(vHist, 128, totalPixels);
    double gamma = calculateGamma(c, cdfVal);

    Mat enhancedV = vChannel.clone();
    applyGammaCorrection(enhancedV, gamma);
    applyVE(enhancedV, vChannel);

    double veMean = cv::mean(enhancedV)[0];
    double sMean = 0.0;
    for (int i = 0; i < 256; ++i) {
        sMean += sHist.at<float>(i) * i;
    }
    sMean /= totalPixels;
    double ves = veMean - sMean;

    adjustSaturation(sChannel, ves);

    vector<Mat> updatedChannels = { hChannel, sChannel, enhancedV };
    Mat updatedHSV, outputImage;
    merge(updatedChannels, updatedHSV);
    cvtColor(updatedHSV, outputImage, COLOR_HSV2BGR);

    auto overallEnd = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(overallEnd - overallStart);
    cout << "[TIMER] Total Time: " << duration.count() / 1000.0 << " seconds" << endl;

    imshow("Display window", outputImage);
    waitKey(0);
    return 0;
}
