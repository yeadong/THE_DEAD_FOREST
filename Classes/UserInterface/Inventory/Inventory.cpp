//
//  Inventory.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 2. 20..
//
//

#include "Inventory.hpp"
#include "ItemSlot.hpp"
#include "ParamLoader.hpp"

namespace realtrick
{
    namespace userinterface
    {
        
        using namespace cocos2d;
        
        Inventory::Inventory() :
        _backgroundView(nullptr),
        _slotScrollView(nullptr),
        _slotSize(Size(Prm.getValueAsFloat("slotWidth"), Prm.getValueAsFloat("slotHeight"))),
        _numOfSlotX(Prm.getValueAsInt("numOfSlotX")),
        _numOfSlotY(Prm.getValueAsInt("numOfSlotY")),
        _isScrolling(false)
        {}
        
        
        Inventory::~Inventory()
        {
            
        }
        
        
        Inventory* Inventory::create()
        {
            Inventory* ret = new (std::nothrow) Inventory();
            if ( ret && ret->init() )
            {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        
        bool Inventory::init()
        {
            if ( !Node::init() )
                return false;
            
            _backgroundView = ui::ImageView::create("inventory_background_view.png", ui::Widget::TextureResType::LOCAL);
            addChild(_backgroundView);
            
            _slotScrollView = ui::ScrollView::create();
            _slotScrollView->setPosition(Vec2(INVENTORY_PAD, INVENTORY_PAD));
            _slotScrollView->setContentSize(Size(Prm.getValueAsFloat("inventoryScrollViewWidth"), Prm.getValueAsFloat("inventoryScrollViewHeight")));
            _slotScrollView->setInnerContainerSize(Size(Prm.getValueAsFloat("inventoryScrollViewWidth"), _slotSize.height * _numOfSlotY + ((_numOfSlotY + 1) * INVENTORY_PAD)));
            _backgroundView->addChild(_slotScrollView);
            
            _slotScrollView->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            _slotScrollView->setBackGroundColor(Color3B::YELLOW);
            _slotScrollView->setScrollBarEnabled(true);
            _slotScrollView->addEventListener([&](Ref* ref, ui::ScrollView::EventType type){
                
                if ( type == ui::ScrollView::EventType::SCROLLING)
                {
                    _isScrolling = true;
                }
                else
                {
                    _isScrolling = false;
                }
                
            });
            
            _slots.resize(_numOfSlotY);
            for(int i = 0 ; i < _numOfSlotY ; ++ i)
            {
                _slots[i].resize(_numOfSlotX);
            }
            
            for(int i = 0 ; i < _numOfSlotY ; ++ i)
            {
                for(int j = 0 ; j < _numOfSlotX ; ++ j)
                {
                    ItemSlot* slot = ItemSlot::create("slotView_n.png", "slotView_s.png", ui::Widget::TextureResType::LOCAL);
                    slot->setAnchorPoint(Vec2::ZERO);
                    slot->setPosition(Vec2((INVENTORY_PAD * (j + 1)) + (j * _slotSize.width),
                                           _slotScrollView->getInnerContainerSize().height - ((i + 1) * INVENTORY_PAD + ((i + 1) * _slotSize.height)) ));
                    slot->addTouchCallback([&](Ref* ref, ui::Widget::TouchEventType type){
                        
                        ItemSlot* self = (ItemSlot*)ref;
                        
                        if ( _isScrolling ) return ;
                        
                        if ( type == ui::Widget::TouchEventType::ENDED )
                        {
                            _disableButtonsExcept(self);
                            self->toggleSelected();
                        }
                        
                    });
                    _slotScrollView->addChild(slot);
                    _slots[i][j] = slot;
                }
            }
            
            return true;
        }
        
        void Inventory::_disableButtonsExcept(ItemSlot* exceptSlot)
        {
            for(int i = 0 ; i < _numOfSlotY ; ++i)
            {
                for(int j = 0 ; j < _numOfSlotX; ++ j)
                {
                    if ( _slots[i][j] == exceptSlot ) continue;
                    
                    _slots[i][j]->tryDisableSelected();
                }
            }
        }
        
    }
}









