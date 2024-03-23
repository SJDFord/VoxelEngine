
#pragma once
template<typename T>
class OpenGLObject
{
public:
  OpenGLObject() : m_obj(T::Create()) {}
  ~OpenGLObject() {T::Destroy(m_obj);}

  operator typename T::value_type() {return m_obj;}

private:
  typename T::value_type m_obj;
};