#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include "ComplexClass.hpp"


#pragma pack(push, 1)
struct BFI
{
    short Signature;
    unsigned int FileSize;
    unsigned int Reserved;
    unsigned int DataOffset;
};

struct InfoHeader
{
    unsigned int Size;
    unsigned int Width;
    unsigned int Height;
    unsigned short Planes;
    unsigned short BitCount;
    unsigned int Compression;
    unsigned int ImageSize;
    unsigned int XpixelsPerM;
    unsigned int YpixelsPerM;
    unsigned int ColorsUsed;
    unsigned int ColorsImportant;

};

struct BMP
{
    BFI bf;
    InfoHeader ih;
};

struct RGB {
    uint8_t r, g, b;
};

#pragma pack(pop)
struct BMPImage
{
    BMP bmp;
    std::vector<unsigned char> dib;

    // Default конструктор
    BMPImage() = default;

    // Конструктор для создания черного изображения
    BMPImage(uint32_t w, uint32_t h) {
        bmp.bf.Signature = 0x4D42;  // "BM" в ASCII
        bmp.bf.FileSize = sizeof(BMP) + w * h * 3;
        bmp.bf.Reserved = 0;
        bmp.bf.DataOffset = sizeof(BMP);

        bmp.ih.Size = sizeof(InfoHeader);
        bmp.ih.Width = w;
        bmp.ih.Height = h;
        bmp.ih.Planes = 1;
        bmp.ih.BitCount = 24;
        bmp.ih.Compression = 0;
        bmp.ih.ImageSize = w * h * 3;
        bmp.ih.XpixelsPerM = 2835;
        bmp.ih.YpixelsPerM = 2835;
        bmp.ih.ColorsUsed = 0;
        bmp.ih.ColorsImportant = 0;

        // Заполняем изображение черным цветом
        dib.resize(bmp.ih.ImageSize, 0);
    }

    void Read(std::fstream &file){
        file.read(reinterpret_cast <char*>(&bmp), sizeof(BMP));
        dib.resize(bmp.ih.ImageSize);
        file.read(reinterpret_cast <char*>(dib.data()), dib.size());
    };

    RGB GetPixel(uint32_t x, uint32_t y) const {
        // uint32_t index = (y * bmp.ih.Width + x) * 3;
        // return {dib[index + 2], dib[index + 1], dib[index]};
        const uint32_t line_bytes = bmp.ih.ImageSize / bmp.ih.Height;
        RGB color;
        color.b = dib[3 * x + line_bytes * y + 0];
        color.g = dib[3 * x + line_bytes * y + 1];
        color.r = dib[3 * x + line_bytes * y + 2];
        return color;
    }

    void SetPixel(uint32_t x, uint32_t y, const RGB& color) {
        const uint32_t line_bytes = bmp.ih.ImageSize / bmp.ih.Height;
        dib[3 * x + line_bytes * y + 0] = color.b;
        dib[3 * x + line_bytes * y + 1] = color.g;
        dib[3 * x + line_bytes * y + 2] = color.r;
    }

    void write(std::ofstream& outfile) const {
        outfile.write(reinterpret_cast<const char*>(&bmp), sizeof(BMP));
        outfile.write(reinterpret_cast<const char*>(dib.data()), dib.size());
    }
};

Complex toC(uint32_t x, uint32_t y, uint32_t w, uint32_t h){
    float xComplex = 3 * (x / float(w)) - 2;
    float yComplex = 2 * (y / float(h)) - 1;
    Complex z(xComplex, yComplex);
    return z;
}

uint32_t M(const Complex c){
    uint32_t n = 0;
    Complex z = c;
    while (z.len() < 4){
        z = z * z + c;
        n++;
        if (n > 15){
            return 0 ;
        }
    }
    return n;
}

void Calc (BMPImage& bmpImage){
    uint32_t w = bmpImage.bmp.ih.Width;
    uint32_t h = bmpImage.bmp.ih.Height;
    for (uint32_t x = 0; x < w; x++){
        for (uint32_t y = 0; y < h; y++){
            Complex c = toC(x, y, w, h);
            uint32_t n = M(c);
            RGB color = {
                static_cast<uint8_t>(n * 15), 
                static_cast<uint8_t>(n * 15), 
                static_cast<uint8_t>(n * 15)
            };
            // RGB color;
            // color.r = color.b = color.b = n * 15;
            bmpImage.SetPixel(x, y, color);
        }
    }
}


int main(){
    // char c1, c2;
    // unsigned int fsize;
    // BFI bf;
    // InfoHeader ih;
    // BMP bmp;
    // BMPImage bmpimage;
    

    // std::ifstream fileName("C:\\Users\\suren\\.vscode\\C++ OOP\\filicheck.bmp", std::ios::binary);
    // // fileName.read(&c1, 1);
    // // fileName.read(&c2, 1);
    // // fileName.read(reinterpret_cast <char*>(&fsize), 4);
    // fileName.read(reinterpret_cast <char*>(&bf), sizeof(BFI));
    // fileName.read(reinterpret_cast <char*>(&ih), sizeof(InfoHeader));
    // fileName.read(reinterpret_cast <char*>(&bmp), sizeof(BMP));
    // // std::cout << bf << std::endl;


    // // std::cout << c1 << std::endl << c2 << std::endl << fsize << std::endl;
    // std::cout << "Size BFI = " << sizeof(BFI) << std::endl 
    // << "Size InfoHeader = " << sizeof(InfoHeader) << std::endl
    // << "Size BMP = " << sizeof(BMP) << std::endl;
    // // << "Size FileName = " << sizeof(fileName) << std::endl;

    BMPImage bmpimage;
    
    std::fstream file("C:\\Users\\suren\\.vscode\\C++ OOP\\filicheck.bmp", std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    bmpimage.Read(file);
    file.close();

    // // Тестируем получение пикселя
    // RGB pixel = bmpimage.GetPixel(0, 0);
    // std::cout << "RGB at (0, 0): (" << (int)pixel.r << ", " << (int)pixel.g << ", " << (int)pixel.b << ")" << std::endl;

    // // Тестируем изменение пикселя
    // bmpimage.SetPixel(0, 0, {255, 0, 0});  // Устанавливаем красный цвет для пикселя (0, 0)

    // Сохраняем изменения в новый файл
    Calc(bmpimage);
    std::ofstream outfile("C:\\Users\\suren\\.vscode\\C++ OOP\\outputCalc.bmp", std::ios::binary);
    bmpimage.write(outfile);
    outfile.close();

    return 0;

}