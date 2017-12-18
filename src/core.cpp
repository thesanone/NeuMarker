#include "core.h"
//#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#include <QFile>
#include <QTextStream>
#include <QPixmapCache>


//bool Core::setConnection(QString server, QString dbName)
//{
//    db = QSqlDatabase::addDatabase("QSQLITE");

//    QString connectionTemplate = "DRIVER={SQL SERVER};SERVER=%1;DATABASE=%2;";
//    QString connectionString = connectionTemplate.arg(server).arg(dbName);
//    db.setDatabaseName(connectionString);

//    return db.open();
//}

Core::Core(QObject *parent):QObject(parent)
{
    //QString server = "3NTO-I7-321\\MSSQLSERVER_SAN1";
    //QString dbName = "neurons";

    //this->setConnection(server, dbName);
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName("E:/SQLite/neurons_v3.db");
    db.setDatabaseName("neurons.db");
    if(!db.open()) qDebug() << "db load error" << db.lastError();
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;");
    /*query.next();
    qDebug() << "first: " << query.value(0).toInt();
    if(!query.value(0).toInt())
    {
        query.clear();
        query.exec("PRAGMA foreign_keys = ON;");
        query.clear();
        query.exec("PRAGMA foreign_keys;");
        query.next();
        qDebug() << "changed: " << query.value(0).toInt();
    }*/

    filter = NULL;
    marker = NULL;
    scene = NULL;
    imageLoaderThread = NULL;
    imageLoader = NULL;
    progressBar = NULL;
    initFilterThread = NULL;
    initFilter = NULL;
}

Core::~Core()
{
    db.close();
    if(filter) delete filter;
        if(marker) delete marker;
    if(progressBar) delete progressBar;
    if(imageLoader) delete imageLoader;
    if(initFilter) delete initFilter;
}

QStringList Core::getAnimalsTypesList()
{
    QStringList result;
    QSqlQuery query;
    query.exec("SELECT * FROM AnimalsTypes");
    int fieldNo = query.record().indexOf("animalType");
    while (query.next())
        result.push_back(query.value(fieldNo).toString());
    return result;
}

QStringList Core::getAnimalsList(QString animalType)
{
    QStringList result;
    QSqlQuery query;
    query.prepare("SELECT * FROM Animals WHERE animalType = ?");
    query.addBindValue(animalType);
    query.exec();
    int fieldNo = query.record().indexOf("typeID");
    while (query.next())
        result.push_back(query.value(fieldNo).toString());
    return result;
}

QStringList Core::getSegmentsList(QString animalType)
{
    QStringList result;
    QSqlQuery query;
    query.prepare("SELECT * FROM Segments WHERE animalType = ?");
    query.addBindValue(animalType);
    query.exec();
    int fieldNo = query.record().indexOf("segmentID");
    while (query.next())
        result.push_back(query.value(fieldNo).toString());
    return result;
}

std::vector<QListWidgetItem *> Core::getSliceWidgetItems(QString animalType, QString typeID, QString segmentID)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Slice WHERE animalType = ? AND animalID = ? AND segmentID = ?");
    query.addBindValue(animalType);
    query.addBindValue(typeID);
    query.addBindValue(segmentID);
    query.exec();

    std::vector<QListWidgetItem*> result;
    while (query.next())
    {
        int idColumn = query.record().indexOf("id");
        int fullNameColumn = query.record().indexOf("fullName");
        int pictColumn = query.record().indexOf("pict");

        int id = query.value(idColumn).toInt();

        QString fullName = query.value(fullNameColumn).toString();

        QByteArray byteArray;
        byteArray = query.value(pictColumn).toByteArray();
        QPixmap pict = QPixmap::fromImage(QImage::fromData(byteArray, "jpg"));

        QListWidgetItem *item = new QListWidgetItem(QIcon(pict), fullName);
        item->setData(Qt::UserRole, id);
        result.push_back(item);
    }
    return result;
}

int Core::add_pre_processing_pict(QString animalType, int animalID, QString segmentID, QString fullName, QString markerType, QPixmap image,
                                  bool isInvert, int slizeNumber, int position, int pict_width, int pict_height, QString comment)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "jpg");

    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO Slice (animalType, animalID, segmentID, fullName, markerType, pict, isInvert, sliceNumber, position, pictWidth, pictHeight, comment)"
                  " VALUES (:animalType, :animalID, :segmentID, :fullName, :markerType, :pict, :isInvert, :sliceNumber, :position, :pictWidth, :pictHeight, :comment);");
    query.bindValue(":animalType", animalType);
    query.bindValue(":animalID", animalID);
    query.bindValue(":segmentID", segmentID);
    query.bindValue(":fullName", fullName);
    query.bindValue(":markerType", markerType);
    query.bindValue(":pict", byteArray);
    query.bindValue(":isInvert", isInvert);
    query.bindValue(":sliceNumber", slizeNumber);
    query.bindValue(":position", position);
    query.bindValue(":pictWidth", pict_width);
    query.bindValue(":pictHeight", pict_height);
    query.bindValue(":comment", comment);
    query.exec();

    if (query.lastError().isValid())
    {
        qDebug() << "error in add pp" << query.lastError();
        qDebug() << "executed Query: " << query.executedQuery();
    }
    return query.numRowsAffected();
}

int Core::updateSliceString(int id, QString animalType, int animalID, QString segmentID, QString fullName,
                                       QString markerType,  bool isInvert, int sliceNumber, int position, QString comment)
{
    QSqlQuery query;
    query.prepare("UPDATE Slice SET animalType = :animalType, animalID = :animalID, segmentID = :segmentID, fullName = :fullName, "
                  "markerType = :markerType, isInvert = :isInvert, sliceNumber = :sliceNumber, position = :position, comment = :comment WHERE id = :id ");
    query.bindValue(":id", id);
    query.bindValue(":animalType", animalType);
    query.bindValue(":animalID", animalID);
    query.bindValue(":segmentID", segmentID);
    query.bindValue(":fullName", fullName);
    query.bindValue(":markerType", markerType);
    query.bindValue(":isInvert", isInvert);
    query.bindValue(":sliceNumber", sliceNumber);
    query.bindValue(":position", position);
    query.bindValue(":comment", comment);
    query.exec();
    return query.numRowsAffected();
}

int Core::delete_pre_processing_string(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Slice WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    return query.numRowsAffected();
}

QList<SingleNeuDetectionResult> Core::getDetectionsResultsListBySliceID(int id)
{
     QSqlQuery query;
     query.prepare("SELECT * FROM NeuronsDetectionResults WHERE SliceID = ?");
     query.addBindValue(id);
     query.exec();

     int idIndex = query.record().indexOf("id");
     int accomplishedIndex = query.record().indexOf("accomplished");
     int neuFoundIndex = query.record().indexOf("neuFound");
     int methodIndex = query.record().indexOf("method");
     int cellsGroupNameIndex = query.record().indexOf("cellsGroupName");

     QList<SingleNeuDetectionResult> result;
     while(query.next())
     {
         SingleNeuDetectionResult item;
         item.id = query.value(idIndex).toInt();
         item.cellsGroupName = query.value(cellsGroupNameIndex).toString();
         item.neuFound = query.value(neuFoundIndex).toInt();
         item.method = query.value(methodIndex).toString();
         item.accomplished = query.value(accomplishedIndex).toString();
         result.push_back(item);
     }
     return result;
}

SingleSlice Core::selectSliceByID(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Slice WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    SingleSlice result;

    int animalTypeIndex = query.record().indexOf("animalType");
    int animalIDIndex = query.record().indexOf("animalID");
    int segmentIDIndex = query.record().indexOf("segmentID");
    int fullNameIndex = query.record().indexOf("fullName");
    int markerTypeIndex = query.record().indexOf("markerType");
    int pictIndex = query.record().indexOf("pict");
    int isInvertIndex = query.record().indexOf("isInvert");
    int sliceNumberIndex = query.record().indexOf("sliceNumber");
    int positionIndex = query.record().indexOf("position");
    int commentIndex = query.record().indexOf("comment");

    if (query.next())
    {
        result.animalType = query.value(animalTypeIndex).toString();
        result.animalID = query.value(animalIDIndex).toInt();
        result.segmentID = query.value(segmentIDIndex).toString();
        result.fullName = query.value(fullNameIndex).toString();
        result.markerType = query.value(markerTypeIndex).toString();
        result.pict = QImage::fromData(query.value(pictIndex).toByteArray(), "jpg");
        result.isInvert = query.value(isInvertIndex).toBool();
        result.sliceNumber = query.value(sliceNumberIndex).toInt();
        result.position = query.value(positionIndex).toInt();
        result.comment = query.value(commentIndex).toString();
    }

    return result;
}

int Core::deleteDetectionResultsString(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM NeuronsDetectionResults WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    return query.numRowsAffected();
}

void Core::upload_experiment(int id, QString name)
{
    QString result = "";
    QSqlQuery query;
    query.prepare("SELECT rect_x, rect_y, rect_width, rect_height FROM NeuronsList WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    while(query.next())
    {
        QString singleString;
        singleString = query.value(0).toString() + " " +
                query.value(1).toString() + " " +
                query.value(2).toString() + " " +
                query.value(3).toString() + " \n";
        result.append(singleString);
    }
    QFile file(name+".txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << result;
}

void Core::upload_experiment_from_scene()
{
    QSqlQuery query;
    query.prepare("SELECT fullName FROM Slice WHERE id = ?");
    query.addBindValue(pre_processing_id);
    query.exec();
    query.next();
    QString image_name = query.value(0).toString();

    QString result = "";

    query.prepare("SELECT rect_x, rect_y, rect_width, rect_height FROM NeuronsList WHERE id = ?");
    query.addBindValue(experiment_id);
    query.exec();
    while(query.next())
    {
        QString singleString;
        singleString = query.value(0).toString() + " " +
                query.value(1).toString() + " " +
                query.value(2).toString() + " " +
                query.value(3).toString() + " \n";
        result.append(singleString);
    }
    QFile file(image_name+".txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << result;
}

int Core::addNewAnimalType(QString name)
{
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO AnimalsTypes (animalType) VALUES (?)");
    query.addBindValue(name);
    query.exec();
    return query.numRowsAffected();
}

int Core::deleteAnimalType(QString name)
{
    QSqlQuery query;
    query.prepare("DELETE FROM AnimalsTypes WHERE animalType = ?");
    query.addBindValue(name);
    query.exec();
    return query.numRowsAffected();
}

int Core::addNewAnimal(QString animalType, int id)
{
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO Animals (animalType, typeID) VALUES (?, ?)");
    query.addBindValue(animalType);
    query.addBindValue(id);
    query.exec();
    return query.numRowsAffected();
}

int Core::deleteAnimal(QString animalType, int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Animals WHERE animalType = ? AND typeID = ?");
    query.addBindValue(animalType);
    query.addBindValue(id);
    query.exec();
    return query.numRowsAffected();
}

int Core::addNewSegment(QString animalType, QString name)
{
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO Segments (animalType, segmentID) VALUES (?, ?)");
    query.addBindValue(animalType);
    query.addBindValue(name);
    query.exec();
    return query.numRowsAffected();
}

int Core::deleteSegment(QString animalType, QString name)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Segments WHERE animalType = ? AND segmentID = ?");
    query.addBindValue(animalType);
    query.addBindValue(name);
    query.exec();
    return query.numRowsAffected();
}

//old query
int Core::add_new_cat(int id, QString comment, QString impacts_segment, QString motion_direct)
{
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO cat (id, comment, impacts_segment, motion_direct) VALUES (?,?,?,?)");
    query.addBindValue(id);
    query.addBindValue(comment);
    query.addBindValue(impacts_segment);
    query.addBindValue(motion_direct);
    query.exec();

    return query.numRowsAffected();
}


void Core::start_process_with_ppi(int id)
{
    pre_processing_id = id;
    QSqlQuery query;
    query.prepare("SELECT fullName FROM Slice where id = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    QString path = query.value(0).toString();

    if (filter) delete filter;

    bool reCreateLinks = false;
    if(!initFilterThread)
    {
        initFilterThread = new QThread(this);
        reCreateLinks = true;
    }
    if(!initFilter)
    {
        initFilter = new LabFilterInit(NULL);
        reCreateLinks = true;
    }
    if(!progressBar)
    {
        progressBar = new EndlessProgressBar("The image is loading, please wait", NULL);
        reCreateLinks = true;
    }

    if(reCreateLinks)
    {
        initFilter->moveToThread(initFilterThread);
        connect(initFilterThread, SIGNAL(started()), initFilter, SLOT(start()));
        connect(initFilter, SIGNAL(imageLoaded()), initFilterThread, SLOT(quit()));
        connect(initFilter, SIGNAL(imageLoaded()), progressBar, SLOT(reject()));
    }

    initFilter->setFileName(path);
    initFilterThread->start();
    progressBar->exec();
    filter = initFilter->getFilter();
}

QGraphicsScene *Core::create_experiment_scene(int experiment_id)
{
    this->experiment_id = experiment_id;
    if(!scene)scene = new QGraphicsScene();
    else scene->clear();
    QSqlQuery query;
    query.prepare("SELECT SliceID FROM NeuronsDetectionResults WHERE id = ?");
    query.addBindValue(experiment_id);
    query.exec();
    query.next();
    pre_processing_id = query.value(0).toInt();

    query.clear();
    query.prepare("SELECT fullName FROM Slice WHERE id = ?");
    query.addBindValue(pre_processing_id);
    query.exec();
    query.next();
    QString path = query.value(0).toString();

    bool reCreateLinks = false;
    if(!imageLoaderThread)
    {
        imageLoaderThread = new QThread(this);
        reCreateLinks = true;
    }
    if(!imageLoader)
    {
        imageLoader = new BigImgLoader(NULL);
        reCreateLinks = true;
    }
    if(!progressBar)
    {
        progressBar = new EndlessProgressBar("The image is loading, please wait", NULL);
        reCreateLinks = true;
    }

    if(reCreateLinks)
    {
        imageLoader->moveToThread(imageLoaderThread);
        connect(imageLoaderThread, SIGNAL(started()), imageLoader, SLOT(start()));
        connect(imageLoader, SIGNAL(imageLoaded()), imageLoaderThread, SLOT(quit()));
        connect(imageLoader, SIGNAL(imageLoaded()), progressBar, SLOT(reject()));
    }

    imageLoader->setFileName(path);
    imageLoaderThread->start();
    progressBar->exec();

    QPixmap *pixmap = imageLoader->getImagePointer();
    if(!pixmap) return scene;
    QGraphicsPixmapItem* pixm = new MouseListener_forQGraphicsPixmapItem(*pixmap);
    scene->addItem(pixm);
    scene->setFocusItem(pixm);

    delete pixmap;
    QPixmapCache::clear();

    query.clear();
    query.prepare("SELECT rect_x, rect_y, rect_width, rect_height FROM NeuronsList WHERE id = ?");
    query.addBindValue(experiment_id);
    query.exec();
    while(query.next())
    {
        scene->addRect(query.value(0).toInt(), query.value(1).toInt(),
                       query.value(2).toInt(), query.value(3).toInt(),QPen(Qt::green));
    }

    scene->setSceneRect(0,0, pixm->pixmap().width(), pixm->pixmap().height());

    return scene;
}

int Core::save_experiment_changes()
{
    QSqlQuery query, query2;
    query2.exec("BEGIN;");
    query.prepare("DELETE FROM NeuronsList WHERE id = ?");
    query.addBindValue(experiment_id);
    query.exec();
    query.clear();

    query.prepare("INSERT INTO NeuronsList (id, rect_x, rect_y, rect_width, rect_height) "
                  "VALUES (:id, :rect_x, :rect_y, :rect_width, :rect_height)");
    query.bindValue(":id", experiment_id);
    QList<QGraphicsItem*> all_items = scene->items();
    int counter = 0;

    for(QList<QGraphicsItem*>::iterator i = all_items.begin(); i != all_items.end(); i++)
    {
        QGraphicsItem* item = *i;
        if(item->type() == 3)
        {
            QGraphicsRectItem* rect_item = (QGraphicsRectItem*)item;
            QRectF rect = rect_item->rect();
            if(rect_item->pen() == QPen(Qt::green))
            {
                query.bindValue(":rect_x", (int)rect.x());
                query.bindValue(":rect_y", (int)rect.y());
                query.bindValue(":rect_width", (int)rect.width());
                query.bindValue(":rect_height", (int)rect.height());
                query.exec();
                if (query.lastError().isValid())
                {
                    qDebug() << query.lastError().text();
                    qDebug() << query.executedQuery();
                    query2.clear();
                    query2.exec("ROLLBACK;");
                    return 0;
                }
                counter++;
            }
        }
    }
    query2.clear();
    query2.exec("COMMIT;");
    query.clear();
    query.prepare("UPDATE NeuronsDetectionResults SET neuFound = ? WHERE id = ?");
    query.addBindValue(counter);
    query.addBindValue(experiment_id);
    query.exec();
    query.clear();
    query.exec("UPDATE experiment_1 SET accomplished = datetime('now','localtime') WHERE id = "+QString::number(experiment_id));
    scene->update();
    return counter;
}

void Core::init_marker(int min_1, int min_2)
{
    if(marker) delete marker;
    marker = new NeuronsMarker(filter->getResultingBinImg());
    marker->set_min_areas(min_1, min_2);
}

QGraphicsScene *Core::create_graphics_scene()
{
//    QTime t;
//    t.start();
    std::vector<cv::Rect> rectList = marker->getRectsList();
//    qDebug() << t.elapsed();


    if(!scene) scene = new QGraphicsScene();
    else scene->clear();

    QGraphicsPixmapItem* pixm = new MouseListener_forQGraphicsPixmapItem(QPixmap::fromImage(this->Mat2QImage(filter->getImage())));

    scene->addItem(pixm);
    scene->setFocusItem(pixm);

    for(std::vector<cv::Rect>::iterator i = rectList.begin(); i != rectList.end(); i++)
    {
        cv::Rect rect = *i;
        scene->addRect(rect.x, rect.y, rect.width, rect.height, QPen(Qt::red));
    }

    return scene;
}

QPixmap Core::get_image_pixmap()
{
    return(QPixmap::fromImage(Mat2QImage(filter->getImage())));
}

QPixmap Core::get_binary_image_pixmap()
{
    return(QPixmap::fromImage(Mat2QImage_gray(filter->getResultingBinImg())));
}


QImage Core::Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows,(int) temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     return dest;
}

QImage Core::Mat2QImage_gray(cv::Mat const& src)
{
     cv::Mat temp = src.clone(); // make the same cv::Mat
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows,(int) temp.step, QImage::Format_Indexed8);
     dest.bits(); // enforce deep copy, see documentation
     return dest;
}

int Core::submit_scene(QString method, QString cellsGroupName, QString comment)
{
    QList<QGraphicsItem*> all_items = scene->items();
    std::vector<cv::Rect> rectList;
    //int pict_width, pict_height;
    for(QList<QGraphicsItem*>::iterator i = all_items.begin(); i != all_items.end(); i++)
    {
        QGraphicsItem* item = *i;
        if(item->type() == 3)
        {
            QGraphicsRectItem* rect_item = (QGraphicsRectItem*)item;
            if(rect_item->pen() == QPen(Qt::green))
            {
                QRectF rec=rect_item->rect();
                rectList.push_back(cv::Rect(rec.x(), rec.y(), rec.width(), rec.height()));
            }
        }
        /*if(item->type() == 7)
        {
            pict_width = (int)item->boundingRect().width();
            pict_height = (int)item->boundingRect().height();
        }*/
    }

    QSqlQuery query;
    QSqlQuery query2;
    query2.exec("BEGIN;");
    query.prepare("INSERT INTO NeuronsDetectionResults (SliceID, method, neuFound, cellsGroupName, comment) "
                  "VALUES (:SliceID, :method, :neuFound, :cellsGroupName, :comment)");
    query.bindValue(":SliceID",pre_processing_id);
    query.bindValue(":method", method);
    query.bindValue(":neuFound", static_cast<quint32>(rectList.size()));
    query.bindValue(":cellsGroupName", cellsGroupName);
    query.bindValue(":comment", comment);


    query.exec();
    if (query.lastError().isValid())
    {
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        query2.clear();
        query2.exec("ROLLBACK;");
        return 0;
    }

    query.clear();
    query.exec("SELECT id FROM NeuronsDetectionResults WHERE rowid=last_insert_rowid()");
    if (query.lastError().isValid())
    {
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        query2.clear();
        query2.exec("ROLLBACK;");
        return 0;
    }

    query.next();
    int topId = query.value(0).toInt();
    query.clear();

    query.prepare("INSERT INTO Lab_filter (id, L_min, L_max, a_min, a_max, b_min, b_max, minArea, minArea2, areaSeparationValue, morphologicalTransformations) VALUES"
                  "(:id, :L_min, :L_max, :a_min, :a_max, :b_min, :b_max, :minArea, :minArea2, :areaSeparationValue, :morphologicalTransformations)");
    query.bindValue(":id", topId);
    std::vector<short> param = filter->getParamArr();
    query.bindValue(":L_min", param[0]);
    query.bindValue(":L_max", param[1]);
    query.bindValue(":a_min", param[2]);
    query.bindValue(":a_max", param[3]);
    query.bindValue(":b_min", param[4]);
    query.bindValue(":b_max", param[5]);
    query.bindValue(":minArea", marker->getMinArea_1());
    query.bindValue(":minArea2", marker->getMinArea_2());
    query.bindValue(":areaSeparationValue", marker->getAreaSeparator());
    query.bindValue(":morphologicalTransformations", "nope.");


    query.exec();
    if (query.lastError().isValid())
    {
        qDebug() << query.lastError().text();
        qDebug() << query.executedQuery();
        query2.clear();
        query2.exec("ROLLBACK;");
        return 0;
    }

    query.clear();

    query.prepare("INSERT INTO NeuronsList (id, rect_x, rect_y, rect_width, rect_height) "
                  "VALUES (:id, :rect_x, :rect_y, :rect_width, :rect_height)");
    query.bindValue(":id", topId);
    for(std::vector<cv::Rect>::iterator i = rectList.begin(); i != rectList.end(); i++)
    {
        cv::Rect rect = *i;
        query.bindValue(":rect_x", rect.x);
        query.bindValue(":rect_y", rect.y);
        query.bindValue(":rect_width", rect.width);
        query.bindValue(":rect_height", rect.height);
        query.exec();
        if (query.lastError().isValid()) {
            qDebug() << query.lastError().text();
            qDebug() << query.executedQuery();
            query2.clear();
            query2.exec("ROLLBACK;");
            return 0;
        }
    }

    query.clear();

    std::vector<unsigned int> hist = filter->getHistogramms();

    query.prepare("INSERT INTO Histograms VALUES "
                  "(:id, :iter, :L, :a, :b)");
    query.bindValue(":id", topId);
    for(int i = 0; i < 256; i++)
    {
        query.bindValue(":iter", i);
        query.bindValue(":L", hist[i]);
        query.bindValue(":a", hist[i+256]);
        query.bindValue(":b", hist[i+512]);
        query.exec();
        if (query.lastError().isValid()) {
            qDebug() << query.lastError().text();
            qDebug() << query.executedQuery();
            query2.clear();
            query2.exec("ROLLBACK;");
            return 0;}
    }
    query2.clear();
    query2.exec("COMMIT;");
    experiment_id = topId;
    return (int)rectList.size();
}

void Core::sceneSetAllRectRed()
{
    QList<QGraphicsItem*> all_items = scene->items();
    for(QList<QGraphicsItem*>::iterator i = all_items.begin(); i != all_items.end(); i++)
    {
        QGraphicsItem* item = *i;
        if(item->type() == 3)
        {
            QGraphicsRectItem* rect_item = (QGraphicsRectItem*)item;
            rect_item->setPen(QPen(Qt::red));
        }
    }
}

void Core::sceneSetAllRectGreen()
{
    QList<QGraphicsItem*> all_items = scene->items();
    for(QList<QGraphicsItem*>::iterator i = all_items.begin(); i != all_items.end(); i++)
    {
        QGraphicsItem* item = *i;
        if(item->type() == 3)
        {
            QGraphicsRectItem* rect_item = (QGraphicsRectItem*)item;
            rect_item->setPen(QPen(Qt::green));
        }
    }
}

void Core::clearLABdetector()
{
    if(filter)
    {
        delete filter;
        filter = NULL;
    }
    if(marker)
    {
        delete marker;
        marker = NULL;
    }
}

void Core::clearScene()
{
    if(scene) scene->clear();
}
