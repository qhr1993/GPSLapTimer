#include "kmlmaploader.h"

KMLMapLoader::KMLMapLoader()
{
    contentHandler = new KMLHandler();
    reader.setContentHandler(contentHandler);
    reader.setErrorHandler(contentHandler);
    targetDir = new QDir("/home/pi/KmlFiles");
    if(!targetDir->exists())
    {
        targetDir->cdUp();
        targetDir->mkdir("KmlFiles");
    }

}

KMLMapLoader::~KMLMapLoader()
{
    delete contentHandler;
    delete targetDir;
    delete kmlInputSource;
}

void KMLMapLoader::loadFile(QString fileName)
{
   file.setFileName(targetDir->absolutePath().append("/").append(fileName));
   qWarning()<<targetDir->absolutePath().append("/").append(fileName);
   kmlInputSource = new QXmlInputSource(&file);
}

void KMLMapLoader::parseFile()
{
   if(!file.open(QFile::ReadOnly | QFile::Text))
   {
       msg = "Loading error.";
       qWarning()<<"Loading error.";
   }
   else
   reader.parse(kmlInputSource);
   file.close();
}
