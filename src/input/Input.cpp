#include "Input.h"
#include "XmlParser.h"

// #include <QDebug>

Input::Input(QObject* parent)
	: QObject{parent}
	, _xml_parser{std::make_unique<XmlParser>()}
{
}

Input::~Input()
{
}

void Input::readFiles(const QStringList& file_names)
{
	auto tracks = _xml_parser->readFile(file_names[0]);
	emit tracksReady(tracks);
}
