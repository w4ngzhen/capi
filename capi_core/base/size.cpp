#include "capi_core/base/size.h"
namespace capi {
Size::Size(int w, int h) {
  this->w_ = w;
  this->h_ = h;
}
int Size::w() const { return this->w_; }
int Size::h() const { return this->h_; }
void Size::SetW(int w) { this->w_ = w; }
void Size::SetH(int h) { this->h_ = h; }
} // namespace capi
