using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace BasicIDE.Helper
{
    internal static class ConsoleTools
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int Left;
            public int Top;
            public int Right;
            public int Bottom;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool AllocConsole();

        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        [DllImport("user32.dll", SetLastError = true)]
        static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int x, int y, int cx, int cy, int flags);

        [DllImport("user32.dll", SetLastError = true)]
        static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);

        [DllImport("user32.dll")]
        public static extern bool GetWindowRect(IntPtr hwnd, ref RECT rectangle);

        private const byte HIDE = 0;
        private const byte SHOW = 5;

        private static int wPosX = 0;
        private static int wPosY = 0;
        private static int wHeight = 0;
        private static int wWidth = 0;

        private static RECT rect;

        private static uint SWP_NOACTIVATE = 0x0010;
        private static uint SWP_NOZORDER = 0x0004;
        private static uint SWP_HIDEWINDOW = 0x0080;
        private static uint SWP_SHOWWINDOW = 0x0040;

        internal static IntPtr GetConsoleHandle()
        {
            return GetConsoleWindow();
        }

        internal static void Show()
        {
            var handle = GetConsoleWindow();
            if (handle != IntPtr.Zero)
            {
                SetWindowPos(handle, IntPtr.Zero, wPosX, wPosY, wWidth, wHeight, (int)SWP_SHOWWINDOW);
                Console.CursorLeft = 0;
                Console.CursorTop = 0;
            }
            else
            {
                AllocConsole();
                Console.WriteLine("Got here");
            }
                
        }

        internal static void Hide()
        {
            var handle = GetConsoleWindow();
            if (handle != IntPtr.Zero)
                SetWindowPos(handle, IntPtr.Zero, wPosX, wPosY, wWidth, wHeight, (int)SWP_HIDEWINDOW);
        }

        internal static void StartupHide()
        {
            var handle = GetConsoleWindow();

            if (handle != IntPtr.Zero)
            {
                GetWindowRect(handle, ref rect);
                wPosX = rect.Left;
                wPosY = rect.Top;
                wWidth = rect.Right - rect.Left + 1;
                wHeight = rect.Bottom - rect.Top + 1;

                SetWindowPos(handle, IntPtr.Zero, 0, 0, 1, 1, (int)SWP_HIDEWINDOW);
            }
                
        }

        internal static void BackgroundRestore()
        {
            var handle = GetConsoleWindow();
            if (handle != IntPtr.Zero)
                SetWindowPos(handle, IntPtr.Zero, wPosX, wPosY, wWidth, wHeight, (int)SWP_NOACTIVATE | (int)SWP_NOZORDER);
        }
    }
}
