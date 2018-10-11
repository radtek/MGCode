#include "stdafx.h"
#include "mfccmd.h"
#include "HL.h"

/*
* Command processing framework
*/

CCommand::CCommand(BOOL canUndoIt, const CString& name)
{
	canUndo = canUndoIt;
	commandName = name;
}

CCommand::~CCommand(void)
{
}

// Command processor
CCommandProcessor::CCommandProcessor(int maxCommands)
{
	maxNoCommands = maxCommands;
	currentCommand = NULL;
	commandFrame = NULL;
	undoMenuItem = ID_EDIT_UNDO;
	redoMenuItem = ID_EDIT_REDO;
}

CCommandProcessor::~CCommandProcessor(void)
{
	ClearCommands();
}

// Pass a command to the processor. The processor calls Do();
// if successful, is appended to the command history unless
// storeIt is FALSE.
BOOL CCommandProcessor::Submit(CCommand *command, BOOL storeIt)
{
	BOOL success = command->Do();

	if(0 == maxNoCommands){
		delete command;
	}
	else{
		if(success && storeIt)
		{
			if (commands.GetCount() == maxNoCommands)
			{
				POSITION first = commands.GetHeadPosition();
				CCommand *firstCommand = (CCommand *)commands.RemoveHead();
				if (first == currentCommand)
					currentCommand = NULL;
				delete firstCommand;
			}
			// We must chop off the current 'branch'
			// so that we're at the end of the command list.
			if (!currentCommand)
				ClearCommands();
			else
			{
				POSITION pos = currentCommand;
				CCommand *cmd = NULL;
				
				(void) commands.GetNext(pos);
				POSITION oldPos = pos;
				
				while (pos && (cmd = (CCommand *)commands.GetNext(pos)))
				{
					commands.RemoveAt(oldPos);
					delete cmd;
					oldPos = pos;
				}
			}
			
			commands.AddTail(command);
			
			currentCommand = commands.GetTailPosition();
			
			SetMenuStrings();
		}
	}

	return success;
}

BOOL CCommandProcessor::Undo()
{
	if (currentCommand)
	{
		CCommand *command = (CCommand *)commands.GetAt(currentCommand);
		if (command->CanUndo())
		{
			BOOL success = command->Undo(TRUE);
			if (success)
			{
				(void) commands.GetPrev(currentCommand);
				SetMenuStrings();
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CCommandProcessor::Redo(void)
{
	CCommand *redoCommand = NULL;
	POSITION redoNode = NULL;
	if (currentCommand && (currentCommand != commands.GetTailPosition()))
	{
		POSITION pos = currentCommand;
		(void) commands.GetNext(pos);
		
		redoCommand = (CCommand *)commands.GetAt(pos);
		redoNode = pos;
	}
	else
	{
		if (commands.GetCount() > 0)
		{
			POSITION pos = commands.GetHeadPosition();
			redoCommand = (CCommand *)commands.GetAt(pos);
			redoNode = pos;
		}
	}
	
	if (redoCommand)
	{
		if (redoCommand->Redo())
		{
			currentCommand = redoNode;
			SetMenuStrings();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CCommandProcessor::CanUndo(void)
{
	if (currentCommand)
		return ((CCommand *)commands.GetAt(currentCommand))->CanUndo();
	else
		return FALSE;
}

BOOL CCommandProcessor::CanRedo(void)
{
	if (((currentCommand == 0) && (commands.GetCount() > 0)) ||
		((currentCommand != 0) && (currentCommand != commands.GetTailPosition())))
		return TRUE;
	else
		return FALSE;
}

void CCommandProcessor::Commit(void)
{
	ClearCommands();
	SetMenuStrings();
}

void CCommandProcessor::SetMenuStrings(void)
{
	if (commandFrame)
	{
		CString strUndo;
		CString strRedo;
		strUndo.LoadString(IDS_PROMPT_UNDO);
		strRedo.LoadString(IDS_PROMPT_REDO);
		
		CString label;
		if (currentCommand)
		{
			CCommand *command = (CCommand *)commands.GetAt(currentCommand);
			CString commandName(command->GetName());
			BOOL canUndo = command->CanUndo();
			
			label=strUndo+commandName+"(&U)\tCtrl+Z";
			SetMenuItem(undoMenuItem, label, canUndo);
			
			// We can redo, if we're not at the end of the history.
			if (currentCommand != commands.GetTailPosition())
			{
				POSITION pos = currentCommand;
				(void) commands.GetNext(pos);
				CCommand *redoCommand = (CCommand *)commands.GetAt(pos);
				CString redoCommandName = redoCommand->GetName();
				label=strRedo+redoCommandName+"(&R)\tCtrl+Y";
				SetMenuItem(redoMenuItem, label, TRUE);
			}
			else
			{
				label=strRedo+"(&R)\tCtrl+Y";
				SetMenuItem(redoMenuItem, label, FALSE);
			}
		}   
		else
		{
			label=strUndo+"(&U)\tCtrl+Z)";
			SetMenuItem(undoMenuItem, label, FALSE);
			
			if (commands.GetCount() == 0)
			{
				label=strRedo+"(&R)\tCtrl+Y";
				SetMenuItem(redoMenuItem, label, FALSE);
			}
			else
			{
				// currentCommand is NULL but there are commands: this means that
				// we've undone to the start of the list, but can redo the first.
				CCommand *redoCommand = (CCommand *)commands.GetHead();
				CString redoCommandName = redoCommand->GetName();
				label=strRedo+redoCommandName+"(&R)\tCtrl+Y";
				SetMenuItem(redoMenuItem, label, TRUE);
			}
		}
	}
}

void CCommandProcessor::ClearCommands(void)
{
	POSITION pos = commands.GetHeadPosition();
	POSITION oldPos = pos;
	CCommand *cmd = NULL;
	while (pos && (cmd = (CCommand *)commands.GetNext(pos)))
	{
		commands.RemoveAt(oldPos);
		delete cmd;
		oldPos = pos;
	}
	
	currentCommand = NULL;
}

void CCommandProcessor::SetMenuItem(UINT menuItem, const CString& label, BOOL enable)
{
	if (!commandFrame)
		return;
	
	HMENU commandEditMenu = commandFrame->GetMenu()->GetSafeHmenu();
	UINT oldFlag = ::GetMenuState(commandEditMenu, menuItem, MF_BYCOMMAND);
	::ModifyMenu(commandEditMenu, menuItem, MF_BYCOMMAND|MF_STRING|oldFlag, menuItem, (const char *)label);
	
	if (enable)
		::EnableMenuItem(commandEditMenu, menuItem, MF_ENABLED);
	else
		::EnableMenuItem(commandEditMenu, menuItem, MF_GRAYED);
}