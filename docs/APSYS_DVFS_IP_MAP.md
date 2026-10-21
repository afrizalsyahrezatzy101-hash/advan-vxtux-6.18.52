# apsys-dvfs.ko: the per-IP sysfs callback map

Recovered from the stock blob's .data relocations (the dev_attr show/store
pointers) and the .text thunk table.

| sysfs attr    | dpu            | vsp                | vdsp                | gsp                |
|---------------|----------------|--------------------|---------------------|--------------------|
| dvfs_table    | get_dvfs_table_show        | get_dvfs_table_show.159  | get_dvfs_table_show.860  | get_dvfs_table_show.860 |
| dvfs_status   | get_dvfs_status_show       | get_dvfs_status_show.165 | get_dvfs_status_show.866 | get_dvfs_status_show.866 |
| idle_index    | get_idle_index_show        | get_idle_index_show.100  | get_idle_index_show.172  | get_idle_index_show.874 |
| work_index    | get_work_index_show        | get_work_index_show.107  | get_work_index_show.177  | get_work_index_show.879 |
| idle_freq     | get_idle_freq_show         | get_idle_freq_show.111   | get_idle_freq_show.182   | get_idle_freq_show.883 |
| work_freq     | get_work_freq_show         | get_work_freq_show.114   | get_work_freq_show.185   | get_work_freq_show.886 |
| hw_dfs_en     | get_hw_dfs_show            | (absent)                | get_hw_dfs_show.188      | get_hw_dfs_show.889 |
| dvfs_enable   | dpu_dvfs_enable_show       | get_dvfs_enable_show     | vdsp_dvfs_enable_show    | gsp_dvfs_enable_show |

The "show" half of the shared callbacks is a THUNK: it is a 4-byte
`b <real function>` at .text+0x12030..0x120a8, one per IP, and the real
function is per-IP too (the ".159"/".165"/... clones).

So the per-IP sysfs set is:

  dpu : dvfs_table dvfs_status idle_index work_index idle_freq work_freq
        hw_dfs_en dvfs_enable
  vsp : dvfs_table dvfs_status idle_index work_index idle_freq work_freq
        dvfs_enable                       (no hw_dfs_en)
  vdsp: dvfs_table dvfs_status idle_index work_index idle_freq work_freq
        hw_dfs_en dvfs_enable
  gsp : dvfs_table dvfs_status idle_index work_index idle_freq work_freq
        hw_dfs_en dvfs_enable

## The exported names the verifier wants

The stock blob's .symtab carries these names (all local, `t`):

  dpu_dvfs_enable_show   dpu_dvfs_enable_store
  vdsp_dvfs_enable_show  vdsp_dvfs_enable_store
  gsp_dvfs_enable_show   gsp_dvfs_enable_store
  get_dvfs_table_show    get_dvfs_status_show   get_dvfs_table_info_show
  get_idle_index_show    set_idle_index_store
  get_work_index_show    set_work_index_store
  get_idle_freq_show     set_idle_freq_store
  get_work_freq_show     set_work_freq_store
  get_hw_dfs_show        set_hw_dfs_store
  get_dvfs_enable_show   set_dvfs_enable_store

plus, per IP, the ops-level callbacks:

  get_dpu_work_index  set_dpu_work_index  get_dpu_idle_index  set_dpu_idle_index
  get_dpu_work_freq   set_dpu_work_freq   get_dpu_idle_freq   set_dpu_idle_freq
  get_dpu_dvfs_table  set_dpu_dvfs_table  get_dpu_dvfs_status
  set_dpu_gfree_wait_delay set_dpu_freq_upd_en_byp set_dpu_freq_upd_delay_en
  set_dpu_freq_upd_hdsk_en set_dpu_dvfs_swtrig_en
  dpu_hw_dfs_en

  ... the same for vsp, vdsp and gsp (vsp has no set_vsp_dvfs_table and no
  get_vsp_dvfs_status in the export list? -> both are present).

## The ops-table slot order (from the .rodata relocation order)

  dpu_dvfs_ops (0xa8 = 21 slots):
    0x00 parse_dt     0x08 dvfs_init    0x10 hw_dfs_en
    0x18 set_work_index  0x20 get_work_index
    0x28 set_idle_index  0x30 get_idle_index
    0x38 set_work_freq   0x40 get_work_freq
    0x48 set_idle_freq   0x50 get_idle_freq
    0x58 get_dvfs_table  0x60 set_dvfs_table
    0x68 (NULL) 0x70 (NULL)
    0x78 get_dvfs_status
    0x80 set_gfree_wait_delay
    0x88 set_freq_upd_en_byp
    0x90 set_freq_upd_delay_en
    0x98 set_freq_upd_hdsk_en
    0xa0 set_dvfs_swtrig_en

  vsp_dvfs_ops (0x98 = 19 slots):
    0x00 parse_dt  0x08 dvfs_init  0x10 hw_dvfs_en
    0x18 set_work_index 0x20 get_work_index
    0x28 set_idle_index 0x30 get_idle_index
    0x38 set_work_freq  0x40 get_work_freq
    0x48 set_idle_freq  0x50 get_idle_freq
    0x58 get_dvfs_table 0x60 (NULL) 0x68 (NULL)
    0x70 get_dvfs_status
    0x78 set_gfree_wait_delay
    0x80 set_freq_upd_en_byp
    0x88 set_freq_upd_delay_en
    0x90 set_freq_upd_hdsk_en   (no set_dvfs_swtrig_en)

  vdsp_dvfs_ops (0xb0 = 22 slots):
    0x00 parse_dt  0x08 parse_pll  0x10 dvfs_init  0x18 hw_dfs_en
    0x20 set_work_index 0x28 get_work_index
    0x30 set_idle_index 0x38 get_idle_index
    0x40 set_work_freq  0x48 get_work_freq
    0x50 set_idle_freq  0x58 get_idle_freq
    0x60 get_dvfs_table 0x68 set_dvfs_table
    0x70 (NULL) 0x78 (NULL)
    0x80 get_dvfs_status
    0x88 set_gfree_wait_delay
    0x90 set_freq_upd_en_byp
    0x98 set_freq_upd_delay_en
    0xa0 set_freq_upd_hdsk_en
    0xa8 set_dvfs_swtrig_en

  gsp_dvfs_ops (0xa8 = 21 slots): same as dpu.

## The per-SoC match data

  dpu:  {"sharkl5" sharkl5_dpu_dvfs_ops} {"roc1" roc1_dpu_dvfs_ops}
       {"sharkl5pro" sharkl5pro_dpu_dvfs_ops} {"qogirl6" qogirl6_dpu_dvfs_ops}
       {"qogirn6pro" qogirn6pro_dpu_dvfs_ops}
  vsp:  {ver, max_level, ops}
       sharkl5(3) roc1(4) sharkl5pro(3) qogirl6(3)
       qogirn6pro(5) qogirn6pro-vpuenc(4)
  vdsp: sharkl5pro, roc1
  gsp:  qogirn6pro

## The DT compatibles

  apsys: sprd,hwdvfs-apsys-{sharkl5,roc1,sharkl5pro,qogirl6}
         sprd,hwdvfs-dpuvsp-qogirn6pro
  dpu:   sprd,hwdvfs-dpu-{sharkl5,roc1,sharkl5pro,qogirl6,qogirn6pro}
  vsp:   sprd,hwdvfs-vsp-{sharkl5,roc1,sharkl5pro,qogirl6,qogirn6pro}
         sprd,hwdvfs-vpuenc-qogirn6pro
  vdsp:  sprd,hwdvfs-vdsp-{sharkl5pro,roc1}
  gsp:   sprd,hwdvfs-gsp-qogirn6pro
