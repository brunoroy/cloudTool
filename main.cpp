#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/operators.hpp>

#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>

#include <core/optionmanager.h>

#define EMTPY_STR ""

static char* renameFolder = "";
static char* filePLY = "";

static OptionEntry entries[] =
{
    { "rename", "r", OPTION_ARG_STRING, &renameFolder, "renames all files in a specific folder." },
    { "convert", "c", OPTION_ARG_STRING, &filePLY, "converts binary PLY files to ASCII PLY file." }
};

void renameFiles(std::string folder)
{
    boost::filesystem::path directory(folder);
    boost::filesystem::directory_iterator it(directory), eod;

    uint fileSize = 0;
    BOOST_FOREACH(boost::filesystem::path const &path, std::make_pair(it, eod))
    {
        if(boost::filesystem::is_regular_file(path))
        {
            std::string oldName = path.filename().string();
            //std::clog << "old name: " << oldName << std::endl;

            uint beginOffset = 5;
            uint endOffset = 4;
            uint offset = beginOffset+endOffset;
            std::string firstPart = std::string("_").append(oldName.substr(0, 4));
            std::string lastPart = oldName.substr(beginOffset, oldName.length()-offset);
            std::string newName = lastPart.append(firstPart).append(".ply");
            //std::clog << "new name: " << newName << std::endl;

            std::string oldFilename = std::string(folder).append(oldName);
            std::string outputFolder("output/");
            std::string newFilename = outputFolder.append(newName);

            std::clog << "oldFilename: " << oldFilename << std::endl;
            std::clog << "newFilename: " << newFilename << std::endl;
            boost::filesystem::rename(oldFilename, newFilename);
            fileSize++;
        }
    }
    std::clog << fileSize << " files renamed." << std::endl;
}

void convertFile(std::string folder)
{
    boost::filesystem::path directory(folder);
    boost::filesystem::directory_iterator it(directory), eod;

    uint fileId = 0;
    BOOST_FOREACH(boost::filesystem::path const &path, std::make_pair(it, eod))
    {
        if(boost::filesystem::is_regular_file(path))
        {
            std::string oldFilePLY = std::string(folder).append(path.filename().string());
            std::clog << "converting " << oldFilePLY << " (" << fileId++ << ")." << std::endl;

            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>());
            pcl::io::loadPLYFile(oldFilePLY, *cloud);

            std::string newFilePLY("output/");
            newFilePLY.append(path.filename().string());
            //newFilePLY = newFilePLY.substr(0, newFilePLY.length()-4);
            //newFilePLY.append("_ASCII.ply");
            pcl::io::savePLYFileASCII(newFilePLY, *cloud);

            std::clog << "file " << newFilePLY << " converted." << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    std::shared_ptr<OptionManager> optionManager;

    optionManager.reset(new OptionManager(argc, argv));
    optionManager->setOptionContext("Simple tool that implement various data operations.");
    optionManager->addUsage("-r <folder>");
    optionManager->addUsage("-c <folder>");
    optionManager->addExample("-r clouds/");
    optionManager->addExample("-c clouds/");
    std::vector<OptionEntry> optionEntries(entries, entries + sizeof(entries)/sizeof(entries[0]));
    optionManager->setOptionEntries(optionEntries);

    OptionParserError *error = NULL;
    if (optionManager->parseOptions(&error))
    {
        if (renameFolder != EMTPY_STR)
            renameFiles(std::string(renameFolder));
        if (filePLY != EMTPY_STR)
            convertFile(std::string(filePLY));
    }

    return 0;
}
