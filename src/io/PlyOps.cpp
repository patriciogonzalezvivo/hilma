#include <iostream> 
#include <fstream>

// #include <sstream>
// #include <cstring>
// #include <iterator>

#define TINYPLY_IMPLEMENTATION
#include "hilma/io/tinyply.h"

#include "hilma/io/PlyOps.h"

namespace hilma {

bool PlyOps::load( const std::string& _filename, Mesh& _mesh ) {

    // std::unique_ptr<std::istream> file_stream;

    // try
    // {

    //     file_stream.reset(new std::ifstream(_filename, std::ios::binary));

    //     if (!file_stream || file_stream->fail()) throw std::runtime_error("file_stream failed to open " + _filename);

    //     file_stream->seekg(0, std::ios::end);
    //     const float size_mb = file_stream->tellg() * float(1e-6);
    //     file_stream->seekg(0, std::ios::beg);

    //     tinyply::PlyFile file;
    //     file.parse_header(*file_stream);

    //     // std::cout << "\t[ply_header] Type: " << (file.is_binary_file() ? "binary" : "ascii") << std::endl;
    //     // for (const auto & c : file.get_comments()) std::cout << "\t[ply_header] Comment: " << c << std::endl;
    //     // for (const auto & c : file.get_info()) std::cout << "\t[ply_header] Info: " << c << std::endl;

    //     // for (const auto & e : file.get_elements()) {
    //     //     std::cout << "\t[ply_header] element: " << e.name << " (" << e.size << ")" << std::endl;
    //     //     for (const auto & p : e.properties) {
    //     //         std::cout << "\t[ply_header] \tproperty: " << p.name << " (type=" << tinyply::PropertyTable[p.propertyType].str << ")";
    //     //         if (p.isList) std::cout << " (list_type=" << tinyply::PropertyTable[p.listType].str << ")";
    //     //         std::cout << std::endl;
    //     //     }
    //     // }

    //     // Because most people have their own mesh types, tinyply treats parsed data as structured/typed byte buffers. 
    //     // See examples below on how to marry your own application-specific data structures with this one. 
    //     std::shared_ptr<PlyData> vertices, normals, colors, texcoords, faces, tripstrip;

    //     // The header information can be used to programmatically extract properties on elements
    //     // known to exist in the header prior to reading the data. For brevity of this sample, properties 
    //     // like vertex position are hard-coded: 
    //     try { vertices = file.request_properties_from_element("vertex", { "x", "y", "z" }); }
    //     catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

    //     try { normals = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }); }
    //     catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

    //     try { colors = file.request_properties_from_element("vertex", { "red", "green", "blue", "alpha" }); }
    //     catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

    //     try { colors = file.request_properties_from_element("vertex", { "r", "g", "b", "a" }); }
    //     catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

    //     try { texcoords = file.request_properties_from_element("vertex", { "u", "v" }); }
    //     catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

    //     // Providing a list size hint (the last argument) is a 2x performance improvement. If you have 
    //     // arbitrary ply files, it is best to leave this 0. 
    //     try { faces = file.request_properties_from_element("face", { "vertex_indices" }, 3); }
    //     catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

    //     // Tristrips must always be read with a 0 list size hint (unless you know exactly how many elements
    //     // are specifically in the file, which is unlikely); 
    //     try { tripstrip = file.request_properties_from_element("tristrips", { "vertex_indices" }, 0); }
    //     catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

    //     file.read(*file_stream);
        
    //     // Example One: converting to your own application types
    //     const size_t numVerticesBytes = vertices->buffer.size_bytes();
    //     mesh.vertices.resize(vertices->count);
    //     std::memcpy(mesh.vertices.data(), vertices->buffer.get(), numVerticesBytes);
    // }
    // catch (const std::exception & e)
    // {
    //     std::cerr << "Caught tinyply exception: " << e.what() << std::endl;
    // }

    return true;
}

bool PlyOps::save( const std::string& _filename, Mesh& _mesh, bool _binnary  ) {

    std::filebuf fb;
    fb.open(_filename.c_str(), std::ios::out | std::ios::binary);
    std::ostream outstream(&fb);
    if (outstream.fail()) throw std::runtime_error("failed to open " + _filename);

    tinyply::PlyFile file;

    file.add_properties_to_element("vertex", { "x", "y", "z" }, 
        tinyply::Type::FLOAT32, _mesh.vertices.size(), reinterpret_cast<uint8_t*>(_mesh.vertices.data()), tinyply::Type::INVALID, 0);

    if (_mesh.hasNormals())
        file.add_properties_to_element("vertex", { "nx", "ny", "nz" },
        tinyply::Type::FLOAT32, _mesh.normals.size(), reinterpret_cast<uint8_t*>(_mesh.normals.data()), tinyply::Type::INVALID, 0);

    if (_mesh.hasTexcoords())
        file.add_properties_to_element("vertex", { "u", "v" },
        tinyply::Type::FLOAT32, _mesh.texcoords.size() , reinterpret_cast<uint8_t*>(_mesh.texcoords.data()), tinyply::Type::INVALID, 0);

    if (_mesh.hasIndices()) {

        if (_mesh.getPrimitive() == TRIANGLES ) {
            std::vector<glm::ivec3> faces = _mesh.getTriangles();
            file.add_properties_to_element("face", { "vertex_indices" },
            tinyply::Type::UINT32, faces.size(), reinterpret_cast<uint8_t*>(faces.data()), tinyply::Type::UINT8, 3);
        }

    }

    file.get_comments().push_back("generated by Hilma");

    // Write file
    file.write(outstream, _binnary);

    return true;
}

}
