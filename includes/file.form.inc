<?php

/**
 * @file
 *
 * Form-related functions.
 */

/**
 * Form building function.
 */
function islandora_marcxml_file_form($form, &$form_state) {
  $form['file'] = array(
    '#type' => 'managed_file',
    '#title' => t('MarcXML File'),
    '#description' => t('A file containing a MarcXML record, to be ' .
      'transformed to MODS.  Select no file to create a record from ' .
      'scratch.'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('xml'),
    ),
  );
  return $form;
}

/**
 * Form submit handler.
 */
function islandora_marcxml_file_form_submit(&$form, &$form_state) {
  // Transform the MarcXML to MODS.
  module_load_include('inc', 'islandora_marcxml');
  $xsl_path = drupal_get_path('module', 'islandora_marcxml') . 
    '/xsl/MARC21slim2MODS3-4.xsl';
  $file = file_load($form_state['values']['file']);
  if ($file) {
    $input_xml = file_get_contents($file->uri);
    file_delete($file);
    $transformed_xml = trim(islandora_marcxml_run_xsl_transform(array(
      'xsl' => $xsl_path,
      'input' => $input_xml,
    )));

    // Get the ingest object
    $object = islandora_ingest_form_get_object($form_state);

    // Create/set the datastream on the ingest object.
    $dsid = 'MODS';
    if (empty($object[$dsid])) {
      $ds = $object->constructDatastream($dsid, 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object[$dsid];
    }
    $ds->content = $transformed_xml;
  }
}

