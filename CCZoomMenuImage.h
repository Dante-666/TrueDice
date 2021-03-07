#include "2d/CCMenuItem.h"

NS_CC_BEGIN

class MenuItemZoomImage : public MenuItem {
    CC_CONSTRUCTOR_ACCESS : MenuItemZoomImage() {}

    virtual ~MenuItemZoomImage() {}

  public:
    static MenuItemZoomImage *create(const std::string &normalImage);

    bool initWithNormalImage(const std::string &normalImage);

    /** Add the zoom-in zoom-out functionality here */
    virtual void selected() override;
    virtual void unselected() override;

  protected:
    Node *_normalImage;
    float _originalScale;

  private:
    CC_DISALLOW_COPY_AND_ASSIGN(MenuItemZoomImage);
};

NS_CC_END
