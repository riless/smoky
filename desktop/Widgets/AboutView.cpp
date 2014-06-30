#include "aboutview.h"

AboutView::AboutView(QWidget *parent) :
    QFrame(parent)
{
    QVBoxLayout *layout = new QVBoxLayout( this );

    layout->addWidget( pageContent = new QLabel() );
    pageContent->setObjectName("aboutPageContent");
    pageContent->setText(
"<center>"
"<img src=\":/res/smoky-ummto.png\">"
"<p><b>Smoky</b> est une application qui permet de gérer une base de données d'individus, et de les identifier grâce à leurs visages.</p>"
"<p>Elle permet également la détection des visages à partir de support divers.</p>"
"L'application est composée de deux parties: \n"
"<ul>"
"<li>client: développé en C++ en utilisant les bibliothèques QT et OpenCV</li>"
"<li>Serveur: développé en Python en utilisant les bibliothèques Twisted et OpenCV</li>"
"</ul>"
"<p>le projet est disponible sur <a href=\"https://github.com/riless/smoky\" style=\"color: #84bd00\" >https://github.com/riless/smoky</a></p><br>"

"<img src=\":/res/team.png\">"
"<table cellspacing=\"20\" style=\"font-weight: 100\">"
"<tr>"
"<td><div><img src=\":/res/amazigh.png\"><h3>Amazigh AMRANE</h3></div></td>"
"<td><div><img src=\":/res/ghilas.png\"><h3>Ghilas BELHADJ</h3></div></td>"
"<td><div><img src=\":/res/massi.png\"><h3>Massinissa HAMIDI</h3></div></td>"
"</tr>"
"</table>"
"</center>"

	);
    pageContent->setOpenExternalLinks( true );
	layout->addStretch(0);

}
