#include "2d/CCMenuItem.h"

NS_CC_BEGIN

class MenuItemZoomImage : public MenuItem {
    CC_CONSTRUCTOR_ACCESS : MenuItemZoomImage() {}

    virtual ~MenuItemZoomImage() {}

  public:
    static MenuItemZoomImage *
    create(const std::string &normalImage,
           const std::string &selectedImage = std::string());

    bool initWithNormalImage(const std::string &normalImage,
                             const std::string &selectedImage = std::string());

    bool isSelected();
    void selectImage();
    void unSelectImage();

    /** Add the zoom-in zoom-out functionality here */
    virtual void selected() override;
    virtual void unselected() override;

  protected:
    Node *_normalImage;
    Node *_selectedImage;
    float _originalScale;
    bool _selected = false;

  private:
    CC_DISALLOW_COPY_AND_ASSIGN(MenuItemZoomImage);
};

NS_CC_END
