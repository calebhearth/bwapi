#include <stdio.h>
#include <tchar.h>

#include <windows.h>

#include <BWAPI\all.h>

#include <string>

using namespace BWAPI;

int _tmain(int argc, _TCHAR* argv[])
{
  try
  {
    const StaticGameData *gd;
    if(!Connect())
    {
      printf("trying to connect...\n");
      while(!Connect())
      {
        Sleep(1000);
      }
    }
    printf("connected\n");

    gd = GetStaticGameData();

    for(;;)
    {
      WaitForEvent();
      int count = 0;
      for each(Util::Indexed<const KnownUnit&> kunit in gd->units)
      {
        if(kunit.item.type == UnitTypeIds::Terran_SCV)
          OrderStop(kunit.index);
        count++;
      }
      char buff[100];
      itoa(count, buff, 10);
      DrawText(10, 10, buff);

      const DynamicGameData *dd = GetDynamicGameData();

      int size;
      size = dd->unitAddEvents.size();
      if(size)
      {
        sprintf(buff, "units added: %d", size);
        PrintText(buff);
      }

      size = dd->unitRemoveEvents.size();
      if(size)
      {
        sprintf(buff, "units removed: %d", size);
        PrintText(buff);
      }
    }
  }
  catch(...)
  {
    printf("exception\n\n");
  }
  system("pause");
  return 0;
}

