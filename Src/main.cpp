

#include <string>
#include <boost/program_options.hpp>

#include "xWebProcessorDefs.h"

#include "logic/xWebProc/xWebProcessor.h"


int main ( int argc, char **argv )
{
    std::cout << "xWebProcessor v" << MAJOR_VERSION << "." << MINOR_VERSION << "." << PATCH_VERSION << std::endl;

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
        std::cout << "Application interrupted because of an abnormal exception caught" << std::endl;
        std::cout << "Exception class: std::exception" << std::endl;
        std::cout << "message: " << e.what() << std::endl;

        return 1;
    }
    catch(...){
        return 1;
    }

    return 0;
}
