#pragma once

#ifndef __QSTDSTREAM_H__
#define __QSTDSTREAM_H__
#include"Common.h"
#include <QtWidgets/QMainWindow>
#include<QtWidgets/qtextedit.h>
#include<QtWidgets/qplaintextedit.h>




class QStdStream :
	public std::basic_streambuf<char>
{
private:
	std::ostream &m_stream;
	std::streambuf *m_old_buf;
	std::string m_string;
	

	QPlainTextEdit* log_window;

public:
	QStdStream(std::ostream &stream, QPlainTextEdit* text_edit);
	~QStdStream();
	auto GetRdBuf() { return m_stream.rdbuf(); }
	void flush();
protected:
	std::vector<char> buffer;
	virtual int_type overflow(int_type v);
	virtual std::streamsize xsputn(const char *p, std::streamsize n);

};

#endif