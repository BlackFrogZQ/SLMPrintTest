#include "../define/paraItem/scanSystemParaItem.h"
#include "../define/paraNode.h"
#include "../para.h"

namespace TIGER_ParaDef
{
    using namespace TIGER_ParaItemDef;
    void initParaRootNode()
    {
        // paraRootNode()->appendChildren(pcParaItem()->currentNode());
        // paraRootNode()->appendChildren(manuParaItem()->currentNode());
        // paraRootNode()->appendChildren(ncParaItem()->currentNode());
        // paraRootNode()->appendChildren(plcParaItem()->currentNode());
        paraService()->setFileName(cnStr("Paras.xml"));
    }
}