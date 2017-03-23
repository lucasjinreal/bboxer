//
// Created by jintian on 17-3-23.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "utils/utils.h"
using namespace std;
using namespace cv;

// bbox struct
struct MouseParams{
    // bbox can have same class, so using multimap
    vector<pair<int, Rect>> class_bbox_pair_vector;
    Mat src_img;
    Rect bbox;
    Point p1;
    Point p2;
    bool clicked;
    string windowName;
};


void check_boundary(Mat image, Rect bbox_rect){
    if (bbox_rect.width>image.cols-bbox_rect.x){
        bbox_rect.width = image.cols - bbox_rect.x;
    }

    if (bbox_rect.height>image.rows - bbox_rect.y){
        bbox_rect.height = image.rows - bbox_rect.y;
    }

    if (bbox_rect.x < 0){
        bbox_rect.x = 0;
    }

    if (bbox_rect.y < 0){
        bbox_rect.height = 0;
    }
}


//void show_image_with_bbox(Mat src_image, vector< Rect> bbox_vector, string windowName){
//    Mat img = src_image.clone();
//    for (vector<Rect>::iterator it = bbox_vector.begin(); it != bbox_vector.end(); ++it) {
//        check_boundary(img, *it);
//        rectangle(img, *it, Scalar(0, 255, 0), 2, 8, 0);
//    }
////    check_boundary(img, bbox);
////    rectangle(img, bbox, Scalar(0, 255, 0), 2, 8, 0);
//    imshow(windowName, img);
//}

void show_and_draw_bbox(Mat src_image, Rect bbox, vector<pair<int, Rect>> class_bbox_pair_vector, string windowName){
    Mat image = src_image.clone();

    int font_face = FONT_HERSHEY_SCRIPT_SIMPLEX;
    double font_scale = 1.5;
    int thickness = 3;

    for (vector<pair<int, Rect>>::iterator it = class_bbox_pair_vector.begin(); it != class_bbox_pair_vector.end(); ++it) {
        int class_index = it->first;
        Rect temp_rect = it->second;
        Point text_origin = Point(temp_rect.x, temp_rect.y);
        putText(image, to_string(class_index), text_origin, font_face, font_scale, Scalar(0, 0, 0), thickness, 8);

        check_boundary(image, temp_rect);
        rectangle(image, temp_rect, Scalar(0, 255, 0), 2, 8, 0);
    }

    check_boundary(image, bbox);
    rectangle(image, bbox, Scalar(0, 255, 0), 2, 8, 0);
    imshow(windowName, image);
}


void onMouse(int event, int x, int y, int f, void* params){
    MouseParams* mp = (MouseParams*) params;
    switch (event){
        case CV_EVENT_LBUTTONDOWN:
            mp->clicked = true;
            mp->p1.x=x;
            mp->p1.y=y;
            mp->p2.x=x;
            mp->p2.y=y;
            break;
        case CV_EVENT_LBUTTONUP:
            mp->p2.x=x;
            mp->p2.y=y;
            mp->clicked= false;
            break;
        case CV_EVENT_MOUSEMOVE:
            if (mp->clicked){
                mp->p2.x=x;
                mp->p2.y=y;
            }
            break;
        default:
            break;
    }
    if (mp->clicked){
        if (mp->p1.x > mp->p2.x){
            mp->bbox.x = mp->p2.x;
            mp->bbox.width = mp->p1.x - mp->p2.x;
        } else{
            mp->bbox.x = mp->p1.x;
            mp->bbox.width = mp->p2.x - mp->p1.x;
        }

        if (mp->p1.y > mp->p2.y){
            mp->bbox.y = mp->p2.y;
            mp->bbox.height = mp->p1.y - mp->p2.y;
        }else{
            mp->bbox.y = mp->p1.y;
            mp->bbox.height = mp->p2.y - mp->p1.y;
        }
    }

    show_and_draw_bbox(mp->src_img, mp->bbox, mp->class_bbox_pair_vector, mp->windowName);
}


void write_bbox_label_to_text(MouseParams *mp, char* save_dir, string save_name){
    int width = mp->src_img.cols;
    int height = mp->src_img.rows;
    vector<pair<int, Rect>> class_bbox_pair_vector = mp->class_bbox_pair_vector;

    vector<string> to_save_bbox_label;
    for (vector<pair<int, Rect>>::iterator it = class_bbox_pair_vector.begin(); it != class_bbox_pair_vector.end(); ++it) {
        int class_index = it->first;
        Rect temp_rect = it->second;

        string x1 = to_string(temp_rect.x);
        string y1 = to_string(temp_rect.y);
        string x2 = to_string(temp_rect.x + temp_rect.width);
        string y2 = to_string(temp_rect.y + temp_rect.height);
        string temp_string = to_string(class_index) + " " + x1 + " " + y1 + " " + x2 + " " + y2;
        to_save_bbox_label.push_back(temp_string);

    }


    ofstream label_out_file(string(save_dir) + '/' + save_name);
    if (label_out_file.is_open()){

        for (const auto &e: to_save_bbox_label){label_out_file << e << "\n";}
//        ostream_iterator<string> label_out_file_iterator(label_out_file, "\n");
//        copy(to_save_bbox_label.begin(), to_save_bbox_label.end(), label_out_file_iterator);
//        label_out_file.close();
    } else{
        cout << "failed to create file, please check your save directory is valid or have right access rights." << endl;
    }
}


void log_bbox(int class_index, Rect bbox){
    cout << "class: " << class_index << "\nbbox rect: ";
    cout << bbox.x << " " << bbox.y << " " << bbox.width << " " << bbox.height << endl;
}

void bound_box(vector<string> image_files, char* label_save_dir){
    for (vector<string>::iterator it=image_files.begin(); it != image_files.end(); ++it) {
        // base file name display on window
        string base_file_name;
        vector<string> temp = split(*it, '/');
        base_file_name = temp.end()[-1];
        string base_file_name_without_extend;
        vector<string> temp_extend = split(base_file_name, '.');
        base_file_name_without_extend = temp_extend.begin()[0];

        long current_index = it - image_files.begin();
        Mat image;
        image = imread(*it, IMREAD_COLOR);
        if( image.empty() ){
            cout << "[WARN] skip not image file: " << base_file_name << endl;
            continue;
        } else{
            cout << "[INFO] solving image: " << base_file_name << " ..." << endl;

            MouseParams *mp = new MouseParams();
            mp->src_img = image;
            mp->bbox = Rect(0, 0, 0, 0);
            mp->windowName = base_file_name;
            mp->clicked = false;
            mp->p1 = Point(0, 0);
            mp->p2 = Point(0, 0);

            // map save class index and according bbox


            namedWindow(base_file_name, WINDOW_AUTOSIZE);
            setMouseCallback(base_file_name, onMouse, (void*) mp);
            imshow(base_file_name, image);

            while (1){
                char c=waitKey();
                if (c == 's'){
                    write_bbox_label_to_text(mp, label_save_dir, base_file_name_without_extend+".txt");
                    cout << "saved bbox label file." << endl;
                    destroyAllWindows(); break;
                }

                if (c == 27){ cout << "bye!" << endl; exit(0);}
                // 'reset' reset current bbox and pop back the last element in bbox vector
                if (c == 'r'){
                    mp->bbox = Rect(0, 0, 0, 0);
                    if (!mp->class_bbox_pair_vector.empty()){
                        mp->class_bbox_pair_vector.pop_back();
                    }
                }

                if (c == '1'){mp->class_bbox_pair_vector.push_back(make_pair(1, mp->bbox)); log_bbox(1, mp->bbox);}
                if (c == '2'){mp->class_bbox_pair_vector.push_back(make_pair(2, mp->bbox)); log_bbox(2, mp->bbox);}
                if (c == '3'){mp->class_bbox_pair_vector.push_back(make_pair(3, mp->bbox)); log_bbox(3, mp->bbox);}
                if (c == '4'){mp->class_bbox_pair_vector.push_back(make_pair(4, mp->bbox)); log_bbox(4, mp->bbox);}



                show_and_draw_bbox(mp->src_img, mp->bbox, mp->class_bbox_pair_vector, mp->windowName);
            }
            //delete mp;

        }

    }
    cout << "all done. keep on detection task!" << endl;
}

