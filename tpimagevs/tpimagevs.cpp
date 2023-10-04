#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int WIN_WIDTH = 1920;
int WIN_HEIGHT = 1080;


int main()
{
    Mat frame;
    int deviceID = 1;  // Utilisez l'ID de la caméra appropriée
    int apiID = CAP_ANY;
    VideoCapture cap;
    cap.open(deviceID, apiID);

    if (!cap.isOpened())
    {
        cout << "Erreur lors de l'ouverture de la caméra" << endl;
        return -1;
    }

    while (1)
    {
        if (!cap.read(frame))
        {
            cout << "Impossible de lire la vidéo" << endl;
            break;
        }

        // Appliquer un flou gaussien pour réduire le bruit
        //GaussianBlur(frame, frame, Size(5, 5), 0);

        // Convertir l'image en espace de couleur HSV
        Mat hsvImage;
        cvtColor(frame, hsvImage, COLOR_BGR2HSV);

        // Définir la plage de couleurs rouges en HSV (teinte)
        Scalar lowerRed = Scalar(255, 255, 255);  // Borne inférieure
        Scalar upperRed = Scalar(0, 0, 0); // Borne supérieure

        // Créer un masque pour les couleurs rouges
        Mat redMask;
        inRange(hsvImage, lowerRed, upperRed, redMask);

        // Modifier la teinte (Hue) des pixels non rouges pour les rendre rouges
        for (int y = 0; y < hsvImage.rows; y++)
        {
            for (int x = 0; x < hsvImage.cols; x++)
            {
                if (redMask.at<uchar>(y, x) == 0)
                {
                    // Modifier la teinte pour faire ressembler à une nuance de rouge
                    hsvImage.at<Vec3b>(y, x)[0] = 0;  // Réglez la teinte à 0 (rouge)
                }
            }
        }

        // Convertir l'image de retour à l'espace de couleur BGR
        cvtColor(hsvImage, frame, COLOR_HSV2BGR);
        namedWindow("frame", WINDOW_NORMAL);
        resizeWindow("frame", WIN_WIDTH, WIN_HEIGHT);
        imshow("frame", frame);

        if (waitKey(2) == 27) // Quitter avec la touche Échap
            break;
    }

    cap.release();
    return 0;
}
