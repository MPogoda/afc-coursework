#include "main_widget.hxx"
#include <QApplication>

int main(int argc, char **argv)
{
  QApplication a(argc, argv);
  Main_Widget w;
  w.show();

  return a.exec();
}
