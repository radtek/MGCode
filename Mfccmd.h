#ifndef MFCCMDH
#define MFCCMDH
/*
 * CCommand
 * A single command. Derive and override at least the Do, Undo members
 */

class CCommand: public CObject
{
protected:
	BOOL canUndo;
	CString commandName;

public:
	CCommand(BOOL canUndoIt = TRUE, const CString& name = "");
	~CCommand(void);

	// Override this to perform a command
	virtual BOOL Do(BOOL bRedo =FALSE) = 0;

	// Override this to undo a command
	virtual BOOL Undo(BOOL bRedo= FALSE) = 0;

	// Override this to redo a command, IF not the same as Do
	virtual BOOL Redo(void) { return Do(TRUE); }

	virtual inline BOOL CanUndo(void) { return canUndo; }
	virtual inline CString GetName(void) { return commandName; }
};

/*
 * CCommandProcessor
 * A command processor through which you control Do/Undo/Redo operations
 */
 
class CCommandProcessor: public CObject
{
 protected:
  CObList commands;
  POSITION currentCommand;
  
  CFrameWnd *commandFrame;
  UINT undoMenuItem;
  UINT redoMenuItem;
 public:
  int maxNoCommands;


  CCommandProcessor(int maxCommands = 10);
  ~CCommandProcessor(void);

  // Pass a command to the processor. The processor calls Do();
  // if successful, it is appended to the command history unless
  // storeIt is FALSE.
  virtual BOOL Submit(CCommand *command, BOOL storeIt = TRUE);
  
  // Undo the last command
  virtual BOOL Undo();
  
  // Redo the last command
  virtual BOOL Redo(void);
  
  // Call these from your OnUpdateUndo, OnUpdateRedo members
  // to update the menu items/toolbar
  virtual BOOL CanUndo(void);
  virtual BOOL CanRedo(void);
  
  // Call to purge the command list, e.g. after a Save operation
  // if you don't want the user to undo/redo
  virtual void Commit(void);

  // Call this to manage an edit menu
  virtual inline void SetFrame(CFrameWnd *frame, UINT undo = ID_EDIT_UNDO, UINT redo = ID_EDIT_REDO)
   { commandFrame = frame; undoMenuItem = undo; redoMenuItem = redo; }
  virtual inline CFrameWnd *GetFrame(void) { return commandFrame; }
  
  // Used internally to set the Undo/Redo menu strings correctly after an operation
  virtual void SetMenuStrings(void);

  // Sets a menu item
  virtual void SetMenuItem(UINT menuItem, const CString& label, BOOL enable);
  
  // Get the list of commands
  inline CObList& GetCommands(void) { return commands; }
  // Get the max number of commands stored
  inline int GetMaxCommands(void) { return maxNoCommands; }
  inline void SetMaxCommands(int max) { maxNoCommands = max; }
  // Get the current position in the command list
  inline POSITION GetCurrentPosition(void) { return currentCommand; }
  // Clear all commands
  virtual void ClearCommands(void);
};

#endif
