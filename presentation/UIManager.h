/**
 * Project Untitled
 */


#ifndef _UIMANAGER_H
#define _UIMANAGER_H

class UIManager {
public: 
    void userPreferences;
    void currentView;
    
/**
 * @param page
 */
void displayPage(Page page);
    
/**
 * @param viewType
 */
void updateView(String viewType);
    
/**
 * @param input
 */
void handleUserInput(Event input);
};

#endif //_UIMANAGER_H