#include "Controller.h"

Controller::Controller()
{
	errorArea = new ErrorArea();
	fileArea = new FileArea();
	textArea = new TextArea();
	fontAndColors = new FontAndColors();
	commandLineArea = new CommandLineArea();
}

Controller::~Controller()
{

}	
