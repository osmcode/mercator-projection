
#include <iomanip>
#include <iostream>
#include <string>

#include <osmium/io/pbf_input.hpp>
#include <osmium/memory/buffer.hpp>
#include <osmium/osm/node.hpp>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: lat_stat PLANET\n";
        std::exit(1);
    }

    std::string filename{argv[1]};

    std::vector<uint64_t> lats(91);
    uint64_t sum = 0;

    osmium::io::Reader reader{filename, osmium::io::read_meta::no, osmium::osm_entity_bits::node};
    while (osmium::memory::Buffer buffer = reader.read()) {
        for (const auto& node : buffer.select<osmium::Node>()) {
            if (node.location().valid()) {
                size_t y = static_cast<size_t>(std::abs(node.location().lat()));
                ++lats[y];
                ++sum;
            }
        }
    }
    reader.close();

    std::cout << "| degree | count | percent |\n";
    std::cout << "| -----: | ----: | ------: |\n";
    std::cout << std::fixed << std::setprecision(2);

    for (size_t i = 0; i <= 90; ++i) {
        std::cout << "| " << i << " | " << lats[i] << " | " << (lats[i] * 100.0 / sum) << "% |\n";
    }

    std::cout << "| total | " << sum << " | 100% |\n";

    return 0;
}

