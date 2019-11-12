#include "lblengine.h"

LblEngine::LblEngine(QObject *parent) : QObject(parent)
{
    dir=QDir::currentPath();
    QDomDocument domDocument;
    QFile fileTemp(dir+"/templates/glabels_szsm.xml");
    domDocument.setContent(&fileTemp, true);
    QDomNodeList list;
    QDomElement main = domDocument.documentElement();
    if ((main.tagName() == "labels-templates")||(main.tagName() == "Glabels-templates")) {
        list = main.elementsByTagName("Template");
        for (int i=0; i<list.size(); i++){
            QString nam=list.at(i).toElement().attribute("brand");
            map[nam]=list.at(i).toElement();
        }
    }
    fileTemp.close();
}

void LblEngine::createLblEd(QString text, QString barcode)
{
    QString templ=QString::fromLocal8Bit("СЗСМ");
    if (map.value(templ).isNull()){
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Ошибка шаблона"),QMessageBox::Ok);
        return;
    }
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
    QDomElement root = doc.createElement("Glabels-document");
    doc.appendChild(root);
    root.appendChild(map.value(templ));
    QDomElement obj = doc.createElement("Objects");
    obj.setAttribute("id", 0);
    obj.setAttribute("rotate", "True");
    root.appendChild(obj);

    obj.appendChild(newText("4mm","35mm","38mm","28.5mm",text,7,true,&doc));
    obj.appendChild(newImage("6.5mm","4mm","33mm","13mm",dir+"/images/simb.png",&doc));
    obj.appendChild(newImage("5.5mm","17.5mm","35mm","2.5mm",dir+"/images/site.png",&doc));
    if (!barcode.isEmpty()) {
        obj.appendChild(newBarcode("4mm","20.5mm","38mm","17mm",barcode,&doc));
    }
    QFile file("lbl.glabels");
    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        doc.save(stream,1);
        file.close();
        system("./runlbl.sh&");
    }
}

void LblEngine::createLblDoz(QString text, QString man, bool num)
{
        QDomDocument doc;
        doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""));
        QDomElement root = doc.createElement("Glabels-document");
        doc.appendChild(root);
        QString templ=QString::fromLocal8Bit("SZSM-02");
        if (map.value(templ).isNull()){
            QMessageBox::critical(NULL,tr("Ошибка"),tr("Ошибка шаблона"),QMessageBox::Ok);
            return;
        }
        root.appendChild(map.value(templ));

        QDomElement obj = doc.createElement("Objects");
        obj.setAttribute("id", 0);
        obj.setAttribute("rotate", "False");
        root.appendChild(obj);
        obj.appendChild(newImage("10mm","5.5mm","39.6mm","17mm",dir+"/images/simb.png",&doc));
        obj.appendChild(newRect("1mm","1mm","58mm","133mm",&doc));
        obj.appendChild(newText("3mm","35.5mm","54mm","88mm",text,11,true,&doc));
        if (!man.isEmpty()){
            obj.appendChild(newText("3mm","122.5mm","54mm","8.5mm",man,14,true,&doc));
        }

        if (num){
            QDomElement f=newText("4.5mm","28.5mm","8.5mm","6mm","${1}",12,false,&doc);
            obj.appendChild(f);

            QDomElement merge = doc.createElement("Merge");
            merge.setAttribute("type","Text/Colon");
            merge.setAttribute("src",dir+"/data.txt");
            root.appendChild(merge);
        }

        QFile file("lbl.glabels");
        if ( file.open( QIODevice::WriteOnly ) ) {
            QTextStream stream( &file );
            doc.save(stream,1);
            file.close();
            system("./runlbl.sh&");
        }
}

QDomElement LblEngine::newImage(QString x, QString y, QString w, QString h, QString fnam, QDomDocument *doc)
{
    QDomElement l=doc->createElement("Object-image");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("src",fnam);
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    return l;
}

QDomElement LblEngine::newBarcode(QString x, QString y, QString w, QString h, QString code, QDomDocument *doc)
{
    QDomElement l =doc->createElement("Object-barcode");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("backend","gnu-barcode");
    l.setAttribute("style","EAN-13");
    l.setAttribute("text","True");
    l.setAttribute("color","0x000000ff");
    l.setAttribute("data",code);
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    return l;
}

QDomElement LblEngine::newText(QString x, QString y, QString w, QString h, QString text, int size, bool bold, QDomDocument *doc)
{
    QDomElement l =doc->createElement("Object-text");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("justify","Left");
    l.setAttribute("valign","Center");
    l.setAttribute("auto_shrink","False");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    QDomElement span = doc->createElement("Span");
    span.setAttribute("color","0x000000ff");
    span.setAttribute("font_family","Liberation Sans");
    span.setAttribute("font_size",size);
    span.setAttribute("font_weight",bold ? "Bold" : "Regular");
    span.setAttribute("font_italic","False");
    span.setAttribute("line_spacing",2.5);
    QStringList list=text.split(QChar('$'));

    for (int i=0; i<list.size(); i++){
        QString t=list.at(i);
        if (t.size()){
            if (t.at(0)==QChar('{')){
                QString nam=t.mid(1,t.indexOf(QChar('}'))-1);
                QDomElement field=doc->createElement("Field");
                field.setAttribute("name",nam);
                span.appendChild(field);
                QString txt=t.mid(t.indexOf(QChar('}'))+1);
                span.appendChild(doc->createTextNode(txt));
            } else {
                span.appendChild(doc->createTextNode(t));
            }
        }
    }

    l.appendChild(span);
    return l;
}

QDomElement LblEngine::newRect(QString x, QString y, QString w, QString h, QDomDocument *doc)
{
    QDomElement l =doc->createElement("Object-box");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("w",w);
    l.setAttribute("h",h);
    l.setAttribute("line_width","0.352778mm");
    l.setAttribute("line_color","0x000000ff");
    l.setAttribute("fill_color","0x00000000");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    return l;
}

QDomElement LblEngine::newLine(QString x, QString y, QString dx, QString dy, QDomDocument *doc)
{
    QDomElement l =doc->createElement("Object-line");
    l.setAttribute("x",x);
    l.setAttribute("y",y);
    l.setAttribute("dx",dx);
    l.setAttribute("dy",dy);
    l.setAttribute("line_width","0.352778mm");
    l.setAttribute("line_color","0x000000ff");
    l.setAttribute("a0",1);
    l.setAttribute("a1",0);
    l.setAttribute("a2",0);
    l.setAttribute("a3",1);
    l.setAttribute("a4",0);
    l.setAttribute("a5",0);
    return l;
}
