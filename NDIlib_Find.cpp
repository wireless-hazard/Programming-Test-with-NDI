#include <iostream>
#include <cassert>
#include <chrono>
#include <unordered_map>
#include <fstream>
#include <Processing.NDI.Lib.h>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#else // _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
#endif // _WIN64
#endif // _WIN32

namespace NDIlib::find
{
    struct NDI_t
    {
    public:

        // Since this is the only class we have from the NDI lib, the init and destroy functions are
        // also been placed in the constructor and destructor
        NDI_t(void)
        {
            // Not required, but "correct" (see the SDK documentation).
            assert(NDIlib_initialize()); // probably should use exceptions, but I haven`t used them in C++ yet

            // We are going to create an NDI finder that locates sources on the network.
            pNDI_find = NDIlib_find_create_v2();
            assert(pNDI_find != nullptr);
        }
        ~NDI_t()
        {
            // Destroy the NDI finder
            NDIlib_find_destroy(pNDI_find);

            // Finished
            NDIlib_destroy();
        }

        bool wait_for_sources(std::chrono::milliseconds timeout)
        {
            return NDIlib_find_wait_for_sources(pNDI_find, timeout.count());
        }

        std::unordered_map<uint32_t, std::string> get_current_sources(void)
        {
            uint32_t no_sources = 0U;
            const NDIlib_source_t* p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources);

            for (uint32_t i = 0U; i < no_sources; i++)
            {
                // Inserts to the map the current found sources, using the value the p_ndi_name
                current_sources.insert({i, std::string{p_sources[i].p_ndi_name}});
            }

            return current_sources;
        }

    private:
        std::unordered_map<uint32_t, std::string> current_sources;
        NDIlib_find_instance_t pNDI_find;
    };
}

int main(int argc, char* argv[])
{
    NDIlib::find::NDI_t source_list_obj;
    std::ofstream list_of_sources_file;

    // Run for one minute
    using namespace std::chrono;
    for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(1);)
    {
        // Wait up till 5 seconds to check for new sources to be added or removed
        if (!source_list_obj.wait_for_sources(5000ms))
        {
            std::cout << "No change to the sources found.\n";
            continue;
        }

        // Get the updated list of sources
        // the map's key is the source number relative of when it got "probed", 
        // and the map's value is the p_ndi_name of the source
        std::unordered_map<uint32_t, std::string> source_list_map = source_list_obj.get_current_sources();

        // Opens the file that will store the last array of sources found
        list_of_sources_file.open("list_of_sources.txt");

        // Display all the sources and write them to the file in a CSV-like format
        std::cout << "Network sources ("<< source_list_map.size() <<" found).\n";

        // Iterates through the map, printing the keys and values and
        // writing the values to the file
        for (const auto& [count, source] : source_list_map)
        {
            std::cout << count + 1 << ". " << source <<"\n";
            list_of_sources_file << source << "; ";
        }

        list_of_sources_file.close();
    }
    
    // Success. We are done
    return 0;
}
