#include "QStdStream.h"


QStdStream::QStdStream(std::ostream &stream, QPlainTextEdit* text_edit) : m_stream(stream)
{
	log_window = text_edit;
	m_old_buf = stream.rdbuf();
	stream.rdbuf(this);
	fflush(stdout);
	buffer.resize(8192);
	setvbuf(stdout, &buffer[0], _IOFBF, 8192);
}


QStdStream::~QStdStream()
{
	// output anything that is left
	if (!m_string.empty())
		log_window->appendPlainText(m_string.c_str());

	m_stream.rdbuf(m_old_buf);
}

void QStdStream::flush()
{
	if (m_string.length() > 2) {
		m_string.erase(m_string.length()-1,1);
		log_window->appendPlainText(m_string.c_str());
		m_string.erase(m_string.begin(), m_string.end());
	}
	
}

std::char_traits<char>::int_type QStdStream::overflow(int_type v)
{
	
	//if (v == '\n')
	//{
	//	//log_window->appendPlainText(m_string.c_str());
	//	//m_string.erase(m_string.begin(), m_string.end());
	//}
	//else
	//	m_string += v;
	m_string += v;
	return v;
}

std::streamsize QStdStream::xsputn(const char * p, std::streamsize n)
{
	m_string.append(p, p + n);

	//int pos = 0;
	//while (pos != std::string::npos)
	//{
	//	pos = m_string.find('\n');
	//	if (pos != std::string::npos)
	//	{
	//		std::string tmp(m_string.begin(), m_string.begin() + pos);
	//		log_window->appendPlainText(tmp.c_str());
	//		m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
	//	}
	//}

	return n;
}
