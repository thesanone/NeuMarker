#ifndef CORE_H
#define CORE_H

#include <QtSql>
#include <QListWidgetItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "neuronsdetection_lab.h"
#include "neuronsmarker.h"
#include "mouselistener_forqgraphicspixmapitem.h"
#include "endlessprogressbar.h"
#include "bigimageloader.h"
#include "labfilterinit.h"

struct SingleSlice
{
    QString animalType;
    int animalID;
    QString segmentID;
    QString fullName;
    QString markerType;
    QImage pict;
    bool isInvert;
    int sliceNumber;
    int position;
    QString comment;
};

struct SingleNeuDetectionResult
{
    int id;
    QString cellsGroupName;
    QString method;
    int neuFound;
    QString accomplished;
};

class Core :public QObject
{
    Q_OBJECT

private:
    QSqlDatabase db;
//    bool setConnection(QString server, QString dbName);
    NeuronsMarker *marker;
    QGraphicsScene *scene;
    int pre_processing_id;
    int experiment_id;

    QThread *imageLoaderThread;
    BigImgLoader *imageLoader;

    EndlessProgressBar *progressBar;

    QThread *initFilterThread;
    LabFilterInit *initFilter;

public:
    neuronsDetection_lab *filter;

    Core( QObject *parent = 0 );
    virtual ~Core();

    //data base processing:
    QStringList getAnimalsTypesList();
    QStringList getAnimalsList(QString animalType);
    QStringList getSegmentsList(QString animalType);
    std::vector<QListWidgetItem *> getSliceWidgetItems(QString animalType, QString typeID, QString segmentID);
    QList<SingleNeuDetectionResult> getDetectionsResultsListBySliceID(int id);

    int add_pre_processing_pict(QString animalType,int animalID, QString segmentID, QString fullName, QString markerType, QPixmap image,
                                bool isInvert, int slizeNumber, int position, int pict_width, int pict_height, QString comment);
    int updateSliceString(int id, QString animalType, int animalID, QString segmentID, QString fullName, QString markerType,
                                bool isInvert, int sliceNumber, int position, QString comment);
    int delete_pre_processing_string(int id);

    int add_new_cat (int id, QString comment, QString impacts_segment, QString motion_direct);

    SingleSlice selectSliceByID(int id);

    int deleteDetectionResultsString(int id);

    void upload_experiment(int id, QString name);
    void upload_experiment_from_scene();

    //int submit_experiment_1();

    // Total settings
    int addNewAnimalType(QString name);
    int deleteAnimalType(QString name);
    int addNewAnimal(QString animalType, int id);
    int deleteAnimal(QString animalType, int id);
    int addNewSegment(QString animalType, QString name);
    int deleteSegment(QString animalType, QString name);

    //neu detection
    void start_process_with_ppi(int id);
    QGraphicsScene *create_experiment_scene(int experiment_id);
    int save_experiment_changes();
    void init_marker(int min_1, int min_2);
    QGraphicsScene *create_graphics_scene();
    QPixmap get_image_pixmap();
    QPixmap get_binary_image_pixmap();

    static QImage Mat2QImage(cv::Mat const& src);
    static QImage Mat2QImage_gray(cv::Mat const& src);

    int submit_scene(QString method, QString cellsGroupName, QString comment);
    void sceneSetAllRectRed();
    void sceneSetAllRectGreen();

public slots:
    void clearLABdetector();
    void clearScene();
};



#endif // CORE_H
