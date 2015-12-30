// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef MUI_AUDIT_H
#define MUI_AUDIT_H

void AuditDialog(HWND hParent, int choice);

// For property sheet Game Audit tab
void InitGameAudit(int gameIndex);
INT_PTR CALLBACK GameAuditDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

int MameUIVerifyRomSet(int game, bool choice);
int MameUIVerifySampleSet(int game);

const char * GetAuditString(int audit_result);
BOOL IsAuditResultKnown(int audit_result);
BOOL IsAuditResultYes(int audit_result);
BOOL IsAuditResultNo(int audit_result);

#endif
