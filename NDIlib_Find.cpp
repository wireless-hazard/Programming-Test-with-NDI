#include <iostream>
#include <cassert>
#include <chrono>
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
        NDI_t(void)
        {
            // Not required, but "correct" (see the SDK documentation).
            assert(NDIlib_initialize()); //probably should use exceptions, but I haven`t use them in C++ yet

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

        NDIlib_find_instance_t get_find_handler(void) {return pNDI_find;}

    private:
        NDIlib_find_instance_t pNDI_find;
    };
}

int main(int argc, char* argv[])
{
    NDIlib::find::NDI_t source_list;

    // Run for one minute
    using namespace std::chrono;
    for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(1);) {
        // Wait up till 5 seconds to check for new sources to be added or removed
        if (!NDIlib_find_wait_for_sources(source_list.get_find_handler(), 5000 /* milliseconds */)) {
            std::cout << "No change to the sources found.\n";
            continue;
        }

        // Get the updated list of sources
        uint32_t no_sources = 0;
        const NDIlib_source_t* p_sources = NDIlib_find_get_current_sources(source_list.get_find_handler(), &no_sources);

        // Display all the sources.
        std::cout << "Network sources ("<< no_sources <<" found).\n";
        for (uint32_t i = 0; i < no_sources; i++)
        {
            std::cout << i + 1 << ". " << p_sources[i].p_ndi_name <<"<n";
        }
    }

    // Success. We are done
    return 0;
}
