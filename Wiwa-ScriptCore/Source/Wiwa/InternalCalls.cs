﻿using System.Runtime.CompilerServices;

namespace Wiwa
{
    using EntityId = System.UInt64;
    internal class InternalCalls
    {
        #region Log
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string log, int param);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector(ref Vector3 param, out Vector3 result);
        #endregion
        #region ECS
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static byte[] GetComponent(EntityId id, System.Type type);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static byte[] AddComponent(EntityId id, System.Type type, byte[] data);
        #endregion
        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsKeyDownIntr(KeyCode key);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsMouseButtonPressedIntr(int button);
        #endregion
    }
}
