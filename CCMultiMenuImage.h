#include "2d/CCMenuItem.h"
#include <vector>

using cocos2d::MenuItem;

class MenuItemMultiImage : public MenuItem {
    CC_CONSTRUCTOR_ACCESS : MenuItemMultiImage() {}

    virtual ~MenuItemMultiImage() {}

  public:
    static MenuItemMultiImage *create(const std::string &img);
    
    bool insertImage(const std::string &img);

    bool activateNextImage();

    /** Add the zoom-in zoom-out functionality here */
    virtual void selected() override;
    virtual void unselected() override;

  protected:
    std::vector<Node *> _imgList;
    int _currIdx = -1;
    float _originalScale;

  private:
    CC_DISALLOW_COPY_AND_ASSIGN(MenuItemMultiImage);
};
