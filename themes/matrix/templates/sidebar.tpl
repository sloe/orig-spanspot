{*
 * $Revision: 17075 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}
<div id="gsSidebar" class="gcBorder1">
  {* Show the sidebar blocks chosen for this theme *}
  {foreach from=$theme.params.sidebarBlocks item=block}
    {g->block type=$block.0 params=$block.1 class="gbBlock"}
  {/foreach}
</div>
<div style="margin-top:12px">Like Spanner Spotter to get updates about new footage</div>
<script src="http://connect.facebook.net/en_US/all.js#xfbml=1"></script><fb:like-box
href="http://www.facebook.com/pages/Spanner-Spotter/117703774975999" width="160" height="100" show_faces="false" stream="false" header="false"></fb:like-box>