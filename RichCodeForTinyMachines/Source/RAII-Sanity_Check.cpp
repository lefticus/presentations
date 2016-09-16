
int main()
{
  while (true) {
    while (*reinterpret_cast<unsigned char*>(53266) != 240) {}
    if ( (*reinterpret_cast<unsigned char*>(56321) & (1 << 4)) == 0) 
    {
      ++*reinterpret_cast<unsigned char*>(53281);
    }
    ++*reinterpret_cast<unsigned char*>(53280);
  }
}

