
// Private definitions for logic module. Handler documentation makes use of the
// following names:
//    globals - 256-byte array of persistent (i.e. saved) game data
//    scratch - 256-byte array of temporary data
//    A, B - arguments for logic conditions
//    X, Y - arguments for logic actions

#pragma once


// handler type for logic conditions
typedef BOOL (*LOGIC_COND_FN)(void *, const LOGIC_ITEM *);

// handler type for logic actions
typedef void (*LOGIC_ACTION_FN)(void *, const LOGIC_ITEM *);


/// <summary>
/// Handler for condition code 0x00.
/// 
/// This condition always returns TRUE.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTrue(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a880, 0x42a800, CondTrue);

/// <summary>
/// Handler for condition code 0x01.
/// 
/// Compare immed/scratch equal: `A == scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCiseq(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a890, 0x42a810, CondCiseq);

/// <summary>
/// Handler for condition code 0x3d.
/// 
/// Compare scratches equal: `scratch[A] == scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCseq(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a8b0, 0x42a830, CondCseq);

/// <summary>
/// Handler for condition code 0x25.
/// 
/// Compare immed/scratch not equal: `A != scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCisne(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a8e0, 0x42a860, CondCisne);

/// <summary>
/// Handler for condition code 0x3f.
/// 
/// Compare scratches not equal: `scratch[A] != scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCsne(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a900, 0x42a880, CondCsne);

/// <summary>
/// Handler for condition code 0x0f.
/// 
/// Compare scratches equal: `scratch[A] == scratch[B]`.
/// 
/// Exists at the same address as `CondCseq`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCsseq(void *ent, const LOGIC_ITEM *item);

/// <summary>
/// Handler for condition code 0x03.
/// 
/// Compare immed/scratch less than, unsigned: `A < scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCisltu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a930, 0x42a8b0, CondCisltu);

/// <summary>
/// Handler for condition code 0x46.
/// 
/// Compare immed/scratch less than, signed: `A < scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCislt(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a950, 0x42a8d0, CondCislt);

/// <summary>
/// Handler for condition code 0x41.
/// 
/// Compare scratches less than, unsigned: `scratch[A] < scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCsltu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a970, 0x42a8f0, CondCsltu);

/// <summary>
/// Handler for condition code 0x48.
/// 
/// Compare scratches less than, signed: `scratch[A] < scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCslt(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a9a0, 0x42a920, CondCslt);

/// <summary>
/// Handler for condition code 0x05.
/// 
/// Compare immed/scratch less/equal, unsigned: `A <= scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCisleu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a9d0, 0x42a950, CondCisleu);

/// <summary>
/// Handler for condition code 0x07.
/// 
/// Compare immed/scratch greater than, unsigned: `A > scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCisgtu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a9f0, 0x42a970, CondCisgtu);

/// <summary>
/// Handler for condition code 0x47.
/// 
/// Compare immed/scratch greater than, signed: `A > scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCisgt(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42aa10, 0x42a990, CondCisgt);

/// <summary>
/// Handler for condition code 0x43.
/// 
/// Compare scratches greater than, unsigned: `scratch[A] > scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCsgtu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42aa30, 0x42a9b0, CondCsgtu);

/// <summary>
/// Handler for condition code 0x49.
/// 
/// Compare scratches greater than, signed: `scratch[A] > scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCsgt(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42aa60, 0x42a9e0, CondCsgt);

/// <summary>
/// Handler for condition code 0x09.
/// 
/// Compare immed/scratch greater/equal, unsigned: `A >= scratch[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCisgeu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42aa90, 0x42aa10, CondCisgeu);

/// <summary>
/// Handler for condition code 0x1e.
/// 
/// Test immed/scratch masked non-zero: `(A & scratch[B]) != 0`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTismnz(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42aab0, 0x42aa30, CondTismnz);

/// <summary>
/// Handler for condition code 0x27.
/// 
/// Test immed/scratch bitwise-and zero: `(A & scratch[B]) == 0`.
/// 
/// Exists at the same address as `CondTismz`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTisaz(void *ent, const LOGIC_ITEM *item);

/// <summary>
/// Handler for condition code 0x22.
/// 
/// Test immed/scratch masked zero: `(A & scratch[B]) == 0`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTismz(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42aad0, 0x42aa50, CondTismz);

/// <summary>
/// Handler for condition code 0x02.
/// 
/// Compare immed/global equal: `A == globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCigeq(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42aaf0, 0x42aa70, CondCigeq);

/// <summary>
/// Handler for condition code 0x3e.
/// 
/// Compare globals equal: `globals[A] == globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCgeq(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42ab80, 0x42aaa0, CondCgeq);

/// <summary>
/// Handler for condition code 0x26.
/// 
/// Compare immed/global not equal: `A != globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCigne(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42ab20, 0x42aad0, CondCigne);

/// <summary>
/// Handler for condition code 0x40.
/// 
/// Compare globals not equal: `globals[A] != globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCgne(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42ab50, 0x42ab00, CondCgne);

/// <summary>
/// Handler for condition code 0x10.
/// 
/// Compare globals equal: `globals[A] == globals[B]`.
/// 
/// Exists at the same address as `CondCgeq`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCggeq(void *ent, const LOGIC_ITEM *item);

/// <summary>
/// Handler for condition code 0x04.
/// 
/// Compare immed/global less than, unsigned: `A < globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCigltu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42abb0, 0x42ab30, CondCigltu);

/// <summary>
/// Handler for condition code 0x58.
/// 
/// Compare immed/global less than, signed: `A < globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCiglt(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42abd0, 0x42ab50, CondCiglt);

/// <summary>
/// Handler for condition code 0x42.
/// 
/// Compare globals less than, unsigned: `globals[A] < globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCgltu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42ac00, 0x42ab80, CondCgltu);

/// <summary>
/// Handler for condition code 0x06.
/// 
/// Compare immed/global less/equal, unsigned: `A <= globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCigleu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42ac30, 0x42abb0, CondCigleu);

/// <summary>
/// Handler for condition code 0x08.
/// 
/// Compare immed/global greater than, unsigned: `A > globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCiggtu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42ac50, 0x42abd0, CondCiggtu);

/// <summary>
/// Handler for condition code 0x59.
/// 
/// Compare immed/global greater than, signed: `A > globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCiggt(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42ac70, 0x42abf0, CondCiggt);

/// <summary>
/// Handler for condition code 0x44.
/// 
/// Compare globals greater than, unsigned: `globals[A] > globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCggtu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42aca0, 0x42ac20, CondCggtu);

/// <summary>
/// Handler for condition code 0x0a.
/// 
/// Compare immed/global greater/equal, unsigned: `A >= globals[B]`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondCiggeu(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42acd0, 0x42ac50, CondCiggeu);

/// <summary>
/// Handler for condition code 0x1f.
/// 
/// Test immed/global masked non-zero: `(A & globals[B]) != 0`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTigmnz(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42acf0, 0x42ac70, CondTigmnz);

/// <summary>
/// Handler for condition code 0x28.
/// 
/// Test immed/global bitwise-and zero: `(A & globals[B]) == 0`.
/// 
/// Exists at the same address as `CondTigmz`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTigaz(void *ent, const LOGIC_ITEM *item);

/// <summary>
/// Handler for condition code 0x23.
/// 
/// Test immed/global masked zero: `(A & globals[B]) == 0`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTigmz(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42ad20, 0x42aca0, CondTigmz);

/// <summary>
/// Handler for condition code 0x2e.
/// 
/// Test immed/scratch xor zero: `(A ^ scratch[B]) == 0`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTisxz(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42b370, 0x42b2f0, CondTisxz);

/// <summary>
/// Handler for condition code 0x2f.
/// 
/// Test immed/global xor zero: `(A ^ globals[B]) == 0`.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTigxz(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42b390, 0x42b310, CondTigxz);

/// <summary>
/// Handler for condition code 0x31.
/// 
/// Test button down: action.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTbdact(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42b3c0, 0x42b340, CondTbdact);

/// <summary>
/// Handler for condition code 0x51.
/// 
/// Test button up: action.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTbuact(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42b3e0, 0x42b360, CondTbuact);

/// <summary>
/// Handler for condition code 0x60.
/// 
/// Test key down: escpae.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result</returns>
EXTERN_C BOOL CondTkdesc(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42c700, 0x42c670, CondTkdesc);
