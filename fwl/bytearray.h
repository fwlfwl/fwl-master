/**
 * @file bytearray.h
 * @brief (serialize/deserialize)
 * @author fwl
 * @email 1822363660@qq.com
 * @date 2023-12-15
 */
#ifndef __FWL_BYTEARRAY_H__
#define __FWL_BYTEARRAY_H__

#include <memory>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>

namespace fwl {

/**
 * @brief serialize or deserialize
 */
class ByteArray {
public:
    typedef std::shared_ptr<ByteArray> ptr;

    /**
     * @brief ByteArray storage Node
     */
    struct Node {
        /**
         * @brief specity node size
         * @param[in] s size of a node
         */
        Node(size_t s);

        /**
         * constructor of no params
         */
        Node();

        /**
         * destructor
         */
        ~Node();

        // pointer of node
        char* ptr;
        // next node
        Node* next;
        // size of a node
        size_t size;
    };

    /**
     * @brief ByteArray Constructor
     * @param[in] base_size：memory block size
     */
    ByteArray(size_t base_size = 4096);

    /**
     * @brief 析构函数
     */
    ~ByteArray();

    /**
     * @brief write fixed lenght int8_t
     */
    void writeFint8  (int8_t value);
    /**
     * @brief write fixed length uint8_t
     */
    void writeFuint8 (uint8_t value);
    /**
     * @brief write fixed length int16_t
     */
    void writeFint16 (int16_t value);
    /**
     * @brief write fixed length uint16_t
     */
    void writeFuint16(uint16_t value);

    /**
     * @brief write fixed length int32_t
     */
    void writeFint32 (int32_t value);

    /**
     * @brief write fixed length uint32_t
     */
    void writeFuint32(uint32_t value);

    /**
     * @brief write fixed length int64_t
     */
    void writeFint64 (int64_t value);

    /**
     * @brief write fixed length uint64_t 
     */
    void writeFuint64(uint64_t value);

    /**
     * @brief write varint32_t 
     */
    void writeInt32  (int32_t value);
    /**
     * @brief write varuint32_t
     */
    void writeUint32 (uint32_t value);

    /**
     * @brief write varint64_t
     */
    void writeInt64  (int64_t value);

    /**
     * @brief write varuint64_t
     */
    void writeUint64 (uint64_t value);

    /**
     * @brief write float 
     */
    void writeFloat  (float value);

    /**
     * @brief write double
     */
    void writeDouble (double value);

    /**
     * @brief write string, use fuint16_t save string length 
     */
    void writeStringF16(const std::string& value);

    /**
     * @brief write string,use fuint16_t int32_t save string length 
     */
    void writeStringF32(const std::string& value);

    /**
     * @brief write string, use fuint64_t save string length 
     */
    void writeStringF64(const std::string& value);

    /**
     * @brief write string,use Varint64 save string length
     */
    void writeStringVint(const std::string& value);

    /**
     * @brief write string, no string length
     */
    void writeStringWithoutLength(const std::string& value);

    /**
     * @brief write int8_t 
     */
    int8_t   readFint8();

    /**
     * @brief read uint8_t
     */
    uint8_t  readFuint8();

    /**
     * @brief read int16_t
     */
    int16_t  readFint16();

    /**
     * @brief read uint16_t
     */
    uint16_t readFuint16();

    /**
     * @brief read int32_t
     */
    int32_t  readFint32();

    /**
     * @brief read uint32_t
     */
    uint32_t readFuint32();

    /**
     * @brief read int64_t
     */
    int64_t  readFint64();

    /**
     * @brief raed uint64_t
     */
    uint64_t readFuint64();

    /**
     * @brief read Varint32
     */
    int32_t  readInt32();

    /**
     * @brief read Varint32(uint32_t)
     */
    uint32_t readUint32();

    /**
     * @brief read Varint64
     */
    int64_t  readInt64();

    /**
     * @brief  read Varint64(uint64_t)
     */
    uint64_t readUint64();

    /**
     * @brief rad float
     */
    float    readFloat();

    /**
     * @brief read double
     */
    double   readDouble();

    /**
     * @brief read string , use uint16_t save string length
     */
    std::string readStringF16();

    /**
     * @brief read string, use uint32_t save string length
     */
    std::string readStringF32();

    /**
     * @brief read string, use uint64_t save string length
     */
    std::string readStringF64();

    /**
     * @brief read string, use Varint64_t save string length
     */
    std::string readStringVint();

    /**
     * @brief clear ByteArray
     * @post m_position = 0, m_size = 0
     */
    void clear();

    /**
     * @brief write data 
     * @param[in] buf:pointer of buf
     * @param[in] size:buf size
     * @post m_position += size, if m_position > m_size, then m_size = m_position
     */
    void write(const void* buf, size_t size);

    /**
     * @brief read data
     * @param[out] buf:pointer of data
     * @param[in] size: the size of buf
     */
    void read(void* buf, size_t size);

    /**
     * @brief read data 
     * @param[out] buf:pointer of data
     * @param[in] size: buf size
     * @param[in] position: start read position
     */
    void read(void* buf, size_t size, size_t position) const;

    /**
     * @brief get ByteArray cur position
     */
    size_t getPosition() const { return m_position;}

    /**
     * @brief set ByteArray cur position
     */
    void setPosition(size_t v);

    /**
     * @brief write to file
     * @param[in] name: file name 
     */
    bool writeToFile(const std::string& name) const;

    /**
     * @brief read data from file
     * @param[in] name: file name 
     */
    bool readFromFile(const std::string& name);

    /**
     * @brief get Node size 
     */
    size_t getBaseSize() const { return m_baseSize;}

    /**
     * @brief get data size which can be read
     */
    size_t getReadSize() const { return m_size - m_position;}

    /**
     * @brief is little endian 
     */
    bool isLittleEndian() const;

    /**
     * @brief set little endian 
     */
    void setIsLittleEndian(bool val);

    /**
     * @brief transform data to string
     */
    std::string toString() const;

    /**
     * @brief transform data to string (Hex format:FF FF FF)
     */
    std::string toHexString() const;

    /**
     * @brief get read buf,save to vector<iovec>
     * @param[out] buffers: save vector of iovec
     * @param[in] len:read size
     * @return actual read size
     */
    uint64_t getReadBuffers(std::vector<iovec>& buffers, uint64_t len = ~0ull) const;

    /**
     * @brief get read buf, save to vector<iovec>(start from position)
     * @param[out] buffers: save vector of iovec
     * @param[in] len read size
     * @param[in] position start read position
     * @return actual read size
     */
    uint64_t getReadBuffers(std::vector<iovec>& buffers, uint64_t len, uint64_t position) const;

    /**
     * @brief get write buf
     * @param[out] buffers: save vector of iovec(can be write)
     * @param[in] len: write size
     * @return actual size
     * @post if(m_position + len) > m_capacity then add capacity
     */
    uint64_t getWriteBuffers(std::vector<iovec>& buffers, uint64_t len);

    /**
     * @brief 返回数据的长度
     */
    size_t getSize() const { return m_size;}
private:
    
    /**
     * @brief expansion(if capacity is enough, do nothing)
     */
    void addCapacity(size_t size);

    /**
     * @brief get data capacity
     */
    size_t getCapacity() const { return m_capacity - m_position;}
private:
    /// base node size
    size_t m_baseSize;
    /// cur position
    size_t m_position;
    /// data capacity 
    size_t m_capacity;
    /// data size 
    size_t m_size;
    /// endian(default:Big Endian)
    int8_t m_endian;
    /// first node
    Node* m_root;
    /// cur node
    Node* m_cur;
};

}

#endif
