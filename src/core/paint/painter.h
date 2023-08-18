namespace std {
  class string;
}

namespace capi {

class Rect;
class Pen;
class Brush;

class Painter {
public:
  Painter();
  Painter(Painter &&) = default;
  Painter(const Painter &) = default;
  Painter &operator=(Painter &&) = default;
  Painter &operator=(const Painter &) = default;
  ~Painter();

  virtual void drawRect(Rect &) = 0;
  virtual void fillRect(Rect &) = 0;
  virtual void drawEllipse(Rect &) = 0;
  virtual void fillEllipse(Rect &) = 0;
  virtual void drawText(std::string &) = 0;
  virtual void setPen(Pen &) = 0;
  virtual void setBrush(Brush &) = 0;
  virtual void save() = 0;
  virtual void restore() = 0;

private:
  
};
}
