/*
 * Copyright (C) 2018 Baseline
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef BASELINE_STREAMS_H_
#define BASELINE_STREAMS_H_


namespace baseline {

class SharedBuffer;

class InputStream
{
public:

  virtual void close() = 0;

  /**
   * Read up to len bytes of data from thie input stream into the buf.
   * An attempt is made to read as many as len bytes, but a smaller number may be read.
   * The number of bytes actually read is returned.
   *
   * This method blocks until input data is available, end of file is detected, or error occures.
   *
   * If no data is available because the stream is at the end of file, the value -1 is returned.
   *
   * @param buf the buffer into which the data is read
   * @param off the start offset in buf at which the data is written
   * @len the maximum number of bytes to read
   * @returns total number of bytes read into the buffer, or -1 if there is no more data because of
   * EOF is reached.
   */
  virtual int read( uint8_t* buf, size_t off, size_t len ) = 0;
};

class OutputStream
{
public:

  virtual void close() = 0;

  /**
   * Writes len bytes from the specified array starting at offset off to the output stream.
   *
   * @param buf the buffer from which data is read
   * @param off the start offset in buf at which the data is read
   * @param len the number of bytes to write
   * @returns number of bytes written, or negitive number indicating an error.
   */
  virtual int write( uint8_t* buf, size_t off, size_t len ) = 0;
};

class NullOutputStream : public OutputStream
{
public:

  void close();
  int write( uint8_t* buf, size_t off, size_t len );

};

class ByteArrayOutputStream : public OutputStream
{
public:

  ByteArrayOutputStream( size_t size = 1024 );
  ~ByteArrayOutputStream();

  /**
   * Return the current size of the buffer
   */
  size_t size() const;

  /**
   * Returns a SharedBuffer containing all the data written to this stream. If the caller
   * wishes to take ownership of the data, then SharedBuffer::bufferFromData,
   * SharedBuffer::acquire SharedBuffer::release should be used.
   */
  void* toSharedBuffer();

  void close();
  int write( uint8_t* buf, size_t off, size_t len );

private:
  size_t mSize;
  SharedBuffer* mBuffer;
};

class ByteArrayInputStream : public InputStream
{
public:
  ByteArrayInputStream( SharedBuffer* buf, size_t offset, size_t len );
  ~ByteArrayInputStream();

  void close();
  int read( uint8_t* buf, size_t off, size_t len );

private:
  SharedBuffer* mBuffer;
  size_t mOffset;
  size_t mLen;

};

class IOProgress
{
public:
  virtual void onProgress( size_t bytesWritten ) = 0;
};

void pump( InputStream& in, OutputStream& out, IOProgress* callback = nullptr, bool closeOutput = true, bool closeInput = true );


} // namespace baseline

#endif // BASELINE_STREAMS_H_