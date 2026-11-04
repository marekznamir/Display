#include "Screens/IScreen.hpp"

template <typename T>
void IScreen<T>::draw()
{
    static_cast<T*>(this)->draw();
}