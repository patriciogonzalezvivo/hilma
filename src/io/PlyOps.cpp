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
    std::unique_ptr<std::istream> file_stream;

    try
    {
        file_stream.reset(new std::ifstream(_filename, std::ios::binary));

        if (!file_stream || file_stream->fail()) throw std::runtime_error("file_stream failed to open " + _filename);

        std::vector<std::string> colors_names, texcoords_names, faces_name;

        tinyply::PlyFile file;
        file.parse_header(*file_stream);

        // std::cout << "\t[ply_header] Type: " << (file.is_binary_file() ? "binary" : "ascii") << std::endl;
        // for (const auto & c : file.get_comments()) std::cout << "\t[ply_header] Comment: " << c << std::endl;
        // for (const auto & c : file.get_info()) std::cout << "\t[ply_header] Info: " << c << std::endl;


        for (const auto & e : file.get_elements()) {
            // std::cout << "\t[ply_header] element: " << e.name << " (" << e.size << ")" << std::endl;

            for (const auto & p : e.properties) {
                // std::cout << "\t[ply_header] \tproperty: " << p.name << " (type=" << tinyply::PropertyTable[p.propertyType].str << ")";
                // if (p.isList) std::cout << " (list_type=" << tinyply::PropertyTable[p.listType].str << ")";
                // std::cout << std::endl;
                if (p.name == "red" || p.name == "green" || p.name == "blue" || p.name == "alpha" ||
                    p.name == "r" || p.name == "g" || p.name == "b" || p.name == "a")
                    colors_names.push_back(p.name);
                else if (   p.name == "u" || p.name == "v" || 
                            p.name == "s" || p.name == "t" ||
                        p.name == "texture_u" || p.name == "texture_v")
                    texcoords_names.push_back(p.name);
                else if (p.name == "vertex_indices" || p.name == "vertex_index")
                    faces_name.push_back(p.name);
                
            }
        }


        // Because most people have their own mesh types, tinyply treats parsed data as structured/typed byte buffers. 
        // See examples below on how to marry your own application-specific data structures with this one. 
        std::shared_ptr<PlyData> vertices, normals, colors, texcoords, faces, tripstrip;

        // The header information can be used to programmatically extract properties on elements
        // known to exist in the header prior to reading the data. For brevity of this sample, properties 
        // like vertex position are hard-coded: 
        try { vertices = file.request_properties_from_element("vertex", { "x", "y", "z" }); }
        catch (const std::exception & e) { }

        try { normals = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }); }
        catch (const std::exception & e) { }

        if (!colors_names.empty()) {
            try { colors = file.request_properties_from_element("vertex", colors_names); }
            catch (const std::exception & e) { }
        }
        
        if (!texcoords_names.empty()) {
            try { texcoords = file.request_properties_from_element("vertex", texcoords_names); }
            catch (const std::exception & ) { }
        }
        
        // Providing a list size hint (the last argument) is a 2x performance improvement. If you have 
        // arbitrary ply files, it is best to leave this 0. 
        if (faces_name.size() == 1) {
            try { faces = file.request_properties_from_element("face", faces_name, 3); }
            catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }
        }

        // // Tristrips must always be read with a 0 list size hint (unless you know exactly how many elements
        // // are specifically in the file, which is unlikely); 
        // try { tripstrip = file.request_properties_from_element("tristrips", { "vertex_indices" }, 0); }
        // catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        file.read(*file_stream);
        
        if (vertices) {
            const size_t numVerticesBytes = vertices->buffer.size_bytes();

            _mesh.m_vertices.resize(vertices->count);
            std::memcpy(_mesh.m_vertices.data(), vertices->buffer.get(), numVerticesBytes);

            if (colors) {
                size_t numChannels = colors_names.size();
                size_t numColors = colors->count;

                float* fClr   = (float *) colors->buffer.get();
                uint8_t* bClr = (uint8_t *) colors->buffer.get();

                for (size_t i = 0; i < numColors; i++) {
                    if (colors->t == tinyply::Type::FLOAT32) {
                        const float r = *fClr++;
                        const float g = *fClr++;
                        const float b = *fClr++;
                        float a = 1.0f;
                        if (numChannels == 4)
                            a = *fClr++;

                        _mesh.addColor(r, g, b, a);
                    }
                    if (colors->t == tinyply::Type::UINT8) {    
                        const int r = *bClr++;
                        const int g = *bClr++;
                        const int b = *bClr++;   
                        int a = 255;
                        if (numChannels == 4)
                            a = *bClr++;

                        _mesh.addColor(r/255.0f, g/255.0f, b/255.0f, a/255.0f); 
                    }
                }
            }

            if (normals) {
                if (normals->t == tinyply::Type::FLOAT32) {
                    const size_t numNormalsBytes = normals->buffer.size_bytes();
                    _mesh.m_normals.resize(normals->count);
                    std::memcpy(_mesh.m_normals.data(), normals->buffer.get(), numNormalsBytes);
                }
            }

            if (texcoords) {
                if (texcoords->t == tinyply::Type::FLOAT32) {
                    const size_t numTexcoordsBytes = texcoords->buffer.size_bytes();
                    _mesh.m_texcoords.resize(texcoords->count);
                    std::memcpy(_mesh.m_texcoords.data(), texcoords->buffer.get(), numTexcoordsBytes);
                }
            }

            if (faces) {
                _mesh.addIndices(reinterpret_cast<int*>(faces->buffer.get()), faces->count * 3);
            }
        }
    }
    catch (const std::exception & e)
    {
        std::cerr << "Caught tinyply exception: " << e.what() << std::endl;
    }

    return true;
}

bool PlyOps::save( const std::string& _filename, Mesh& _mesh, bool _binnary  ) {

    std::filebuf fb;
    fb.open(_filename.c_str(), std::ios::out | std::ios::binary);
    std::ostream outstream(&fb);
    if (outstream.fail()) throw std::runtime_error("failed to open " + _filename);

    tinyply::PlyFile file;

    file.add_properties_to_element("vertex", { "x", "y", "z" }, 
        tinyply::Type::FLOAT32, _mesh.m_vertices.size(), reinterpret_cast<uint8_t*>(_mesh.m_vertices.data()), tinyply::Type::INVALID, 0);

    if (_mesh.hasColors())
        file.add_properties_to_element("vertex", { "r", "g", "b", "a" },
        tinyply::Type::FLOAT32, _mesh.m_colors.size(), reinterpret_cast<uint8_t*>(_mesh.m_colors.data()), tinyply::Type::INVALID, 0);

    if (_mesh.hasNormals())
        file.add_properties_to_element("vertex", { "nx", "ny", "nz" },
        tinyply::Type::FLOAT32, _mesh.m_normals.size(), reinterpret_cast<uint8_t*>(_mesh.m_normals.data()), tinyply::Type::INVALID, 0);

    if (_mesh.hasTexcoords())
        file.add_properties_to_element("vertex", { "u", "v" },
        tinyply::Type::FLOAT32, _mesh.m_texcoords.size() , reinterpret_cast<uint8_t*>(_mesh.m_texcoords.data()), tinyply::Type::INVALID, 0);

    if (_mesh.hasIndices()) {

        if (_mesh.getMode() == TRIANGLES ) {
            file.add_properties_to_element("face", { "vertex_indices" },
            tinyply::Type::INT32, _mesh.m_indices.size()/3 , reinterpret_cast<uint8_t*>(_mesh.m_indices.data()), tinyply::Type::UINT8, 3);
        }

    }

    file.get_comments().push_back("generated by Hilma");

    // Write file
    file.write(outstream, _binnary);

    return true;
}

}
