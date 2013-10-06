#include <QtCore>
#include <QtGui>

#include "mandraworks/core/Log/Log.h"
#include "mandraworks/core/Log/LogManager.h"
#include "mandraworks/core/Log/LogListenerAll.h"
#include "mandraworks/core/Log/LogSinkSessionFile.h"
#include "mandraworks/core/Log/LogSinkStdOut.h"
#include "mandraworks/core/Log/LogFormaterSSV.h"
#include "mandraworks/core/Log/LogFormaterSerial.h"

#include <string>
#include <boost/program_options.hpp>

#include "xWebProcessorDefs.h"
#include "app/common/ApplicationEnvironment.h"

#include "app/common/CommandLineParams.h"

#include "logic/xWebProc/xWebProcessor.h"


int main ( int argc, char **argv )
{
    ApplicationEnvironment appEnv;

    // setup log environment
    // ********* start **********
    mandraworks::core::log::LogListener* listener = 0;
    mandraworks::core::log::Log::initialise();
    mandraworks::core::log::Log::manager()->registerListener( listener = new mandraworks::core::log::LogListenerAll(mandraworks::core::log::Log::manager()));

    mandraworks::core::log::LogSinkSessionFile* sink = new mandraworks::core::log::LogSinkSessionFile(mandraworks::core::log::Log::manager());
    sink->setOutputFolder(appEnv.applicationLogFolder(COMPANY_NAME, APPLICATION_NAME));
    sink->setFileBaseName(QString("%1_session.log").arg(APPLICATION_NAME).toLower());
    sink->connect(listener);
    sink->setFormater(new mandraworks::core::log::LogFormaterSSV(mandraworks::core::log::Log::manager()));
    sink->startLogging();

    mandraworks::core::log::LogSinkStdOut* sink2 = new mandraworks::core::log::LogSinkStdOut(mandraworks::core::log::Log::manager());
    sink2->connect(listener);
    sink2->setFormater(new mandraworks::core::log::LogFormaterSerial(mandraworks::core::log::Log::manager()));

    // ********* end ************

    //QCoreApplication object is defined outside de try block. If defined inside, the
    //catch of an exception will result in its destruction.  So there
    //is no way to inform the user with a GUI widget.
    QCoreApplication app(argc, argv);

    try{
        // Declare the supported options.
        boost::program_options::options_description desc("Allowed options");
        desc.add_options()
                ("help", "shows the help for the program options")
                ("projectfile", boost::program_options::value<std::string>(), "Sepcify project file")
                ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
        }
        else if ( vm.count("projectfile")) {
            std::string projectfile = vm["projectfile"].as<std::string>();

            xWebProcessor processor;

            processor.setProjectFilePath(projectfile);
            processor.run();
        }

    }
    catch(const std::exception &e){
        QString errMsg="Application interrupted because of an abnormal exception caught\n";
        errMsg+="Exception class: std::exception";
        errMsg+=QString("message: %1 \n").arg(e.what());
        //ERROUT_GUI(errMsg);
        sink->stopLogging(true);
        return 1;
    }
    catch(...){
        sink->stopLogging(true);
        return 1;
    }

    sink->stopLogging(false);
    return 0;
}
