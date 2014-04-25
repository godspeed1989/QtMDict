#include "mainwindow.h"
#include "mdictparser.hh"
#include <QApplication>
#include <QDebug>
using namespace Mdict;

//#define MdxFile "D:\\MDictPC\\Library\\bible.mdx"
#define MdxFile   "D:\\MDictPC\\Library\\C语言库函数.mdx"
#define FindWord  "itoa"

map< QString, MdictParser::RecordInfo > DictIndex;

class MyRecordHandler : public MdictParser::RecordHandler
{
public:
    virtual void handleRecord( QString const & name, MdictParser::RecordInfo const & recordInfo )
    {
        DictIndex.insert(pair< QString, MdictParser::RecordInfo >(name, recordInfo));
    }
};

void parse_mdx()
{
    bool ret;
    MdictParser mdxparser;

    ret = mdxparser.open(MdxFile);
    if (ret == false)
    {
        qDebug() << "open error" << endl;
        return;
    }

    qDebug() << "title: " << mdxparser.title() << endl;
    qDebug() << "describe: " << mdxparser.description() << endl;
    qDebug() << "encoding : " << mdxparser.encoding() << endl;
    qDebug() << "word count: " << mdxparser.wordCount() << endl;

    MdictParser::StyleSheets const & styleSheets = mdxparser.styleSheets();
    qDebug() << "style sheets: " << styleSheets.size() << endl;
    for ( MdictParser::StyleSheets::const_iterator iter = styleSheets.begin();
          iter != styleSheets.end(); iter++ )
    {
        qint32 idx = iter->first;
        QString styleBegin( iter->second.first );
        QString styleEnd( iter->second.second );
        qDebug() << idx << endl;
        qDebug() << styleBegin << endl;
        qDebug() << styleEnd << endl;
        qDebug() << "--------------------------" << endl;
    }

    // enumerating word and its definition
    MdictParser::HeadWordIndex headWordIndex;
    MyRecordHandler recordHandler;
    while ( mdxparser.readNextHeadWordIndex( headWordIndex ) )
    {
        mdxparser.readRecordBlock( headWordIndex, recordHandler );
    }
    qDebug() << "Total words: " << DictIndex.size() << endl;

    // find word
    map< QString, MdictParser::RecordInfo >::iterator it;
    it = DictIndex.find(QString(FindWord));
    if (it == DictIndex.end())
    {
        qDebug() << "Can't find word" << endl;
        return;
    }
    // open file
    QFile dictFile;
    dictFile.setFileName( QString::fromUtf8( MdxFile ) );
    dictFile.open( QIODevice::ReadOnly );
    if (dictFile.isOpen() == false)
    {
        qDebug() << "Open File Error" << endl;
        return;
    }
    // extract article
    const MdictParser::RecordInfo & recordInfo = it->second;
    ScopedMemMap compressed( dictFile, recordInfo.compressedBlockPos,
                             recordInfo.compressedBlockSize );
    if ( !compressed.startAddress() )
    {
        qDebug() << "Corrupt Dictionary" << endl;
        return;
    }
    QByteArray decompressed;
    mdxparser.parseCompressedBlock( recordInfo.compressedBlockSize,
                                    ( char * )compressed.startAddress(),
                                    recordInfo.decompressedBlockSize, decompressed );
    // decompressed
    QString article = MdictParser::toUtf16( mdxparser.encoding(),
                                            decompressed.constData() + recordInfo.recordOffset,
                                            recordInfo.recordSize );
    article = MdictParser::substituteStylesheet( article, styleSheets );
    //article = filterResource( article );
    qDebug() << "+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
    qDebug() << article;
    qDebug() << "+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    parse_mdx();
    exit(0);
    return a.exec();
}
